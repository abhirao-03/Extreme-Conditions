#include "simulation.h"

vec4 Simulation::m_GetSlopeMeasure(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right)
    {
        vec4 t_vec3FirstTerm = (1.0/2.0) * (1 + m_dOmega) * (U_Center - U_Left);
        vec4 t_vec3SecondTerm = (1.0/2.0) * (1 - m_dOmega) * (U_Right - U_Center);

        return t_vec3FirstTerm + t_vec3SecondTerm;
    }

void Simulation::m_ReconstructData(int f_iDirection)
    {
        std::vector<std::vector<vec4>> t_vConserved = m_vec_dU;
        
        for (int j = 0; j < m_iNumGhostCells + m_iYNumPoints; j++)
            {
                for (int i = 0; i < m_iNumGhostCells + m_iXNumPoints; i++)
                    {
                        m_vec_dU[j][i] = m_GetPrimitives(t_vConserved[j][i]);
                    }
            }
        
        m_vec_LeftReconstructed = m_vec_dU;
        m_vec_RightReconstructed = m_vec_dU;

        for (int j = m_iNumGhostCells/2; j < m_iYNumPoints + m_iNumGhostCells/2; j++)
            {
                for (int i = m_iNumGhostCells/2; i < m_iXNumPoints + m_iNumGhostCells/2; i++)
                    {
                        vec4 U_Left, U_Center, U_Right;
                        
                        if (f_iDirection == 0)
                            {
                                U_Left   = m_vec_dU[j][i - 1];
                                U_Center = m_vec_dU[j][i];
                                U_Right  = m_vec_dU[j][i + 1];
                            }
                        else
                            {
                                U_Left   = m_vec_dU[j - 1][i];
                                U_Center = m_vec_dU[j][i];
                                U_Right  = m_vec_dU[j + 1][i];
                            }
                       
                        vec4 l_vDeltaI  = m_GetSlopeMeasure(U_Left, U_Center, U_Right);
                        vec4 l_vLimiter = (this->*m_LimitingFunction)(U_Left, U_Center, U_Right);
                        
                        m_vec_LeftReconstructed[j][i]  = m_vec_dU[j][i] - (1.0/2.0)*l_vLimiter*l_vDeltaI;
                        m_vec_RightReconstructed[j][i] = m_vec_dU[j][i] + (1.0/2.0)*l_vLimiter*l_vDeltaI;
                    }
        }
        
        for (int j = 0; j < m_iNumGhostCells + m_iYNumPoints; j++)
            {
                for (int i = 0; i < m_iNumGhostCells + m_iXNumPoints; i++)
                    {
                        m_vec_LeftReconstructed[j][i]  = m_GetConserved(m_vec_LeftReconstructed[j][i]);
                        m_vec_RightReconstructed[j][i] = m_GetConserved(m_vec_RightReconstructed[j][i]);
                    }
            }
        m_vec_dU = t_vConserved;
    }
    
void Simulation::m_GetReconstructedFluxes(int f_iDirection)
    {
        FluxFunction t_CurrentFluxFunction;
        double t_dDeltaDirection;
        
        if (f_iDirection == 0)
            {
                t_CurrentFluxFunction = &Simulation::m_vXFlux;
                t_dDeltaDirection = m_dDeltaX;
            }
        else
            {
                t_CurrentFluxFunction = &Simulation::m_vYFlux;
                t_dDeltaDirection = m_dDeltaY;
            }
        
        double spatial_over_dt = (t_dDeltaDirection/m_dDeltaT);
        double dt_over_spatial = (m_dDeltaT / t_dDeltaDirection);
        
        for (int j = 0; j < m_iYNumPoints + m_iNumGhostCells/2; j++)
            {
                for (int i = 0; i < m_iXNumPoints + m_iNumGhostCells/2; i++)
                    {
                        if (f_iDirection == 0)
                            {   
                                vec4 t_dULeft  = m_vec_RightReconstructed[j][i];
                                vec4 t_dURight = m_vec_LeftReconstructed[j][i+1];
                                
                                vec4 t_dFluxLeft  = (this->*t_CurrentFluxFunction)(t_dULeft);
                                vec4 t_dFluxRight = (this->*t_CurrentFluxFunction)(t_dURight);
                                
                                vec4 FluxLaxFriedrichs = 0.5 * (t_dFluxLeft + t_dFluxRight) + 0.5 * spatial_over_dt * (t_dULeft - t_dURight);
                                
                                vec4 U_Richtmyer = 0.5 * (t_dULeft + t_dURight) + 0.5 * dt_over_spatial * (t_dFluxLeft - t_dFluxRight);
                                vec4 FluxRichtmter = (this->*t_CurrentFluxFunction)(U_Richtmyer);
                                
                                m_vec_dFluxesReconstructed[j][i] = 0.5 * (FluxLaxFriedrichs + FluxRichtmter);
                            }
                        else 
                            {
                                vec4 t_dULeft  = m_vec_RightReconstructed[j][i];
                                vec4 t_dURight = m_vec_LeftReconstructed[j+1][i];
                                
                                vec4 t_dFluxLeft  = (this->*t_CurrentFluxFunction)(t_dULeft);
                                vec4 t_dFluxRight = (this->*t_CurrentFluxFunction)(t_dURight);
                                
                                vec4 FluxLaxFriedrichs = 0.5 * (t_dFluxLeft + t_dFluxRight) + 0.5 * spatial_over_dt * (t_dULeft - t_dURight);
                                
                                vec4 U_Richtmyer = 0.5 * (t_dULeft + t_dURight) + 0.5 * dt_over_spatial * (t_dFluxLeft - t_dFluxRight);
                                vec4 FluxRichtmter = (this->*t_CurrentFluxFunction)(U_Richtmyer);
                                
                                m_vec_dFluxesReconstructed[j][i] = 0.5 * (FluxLaxFriedrichs + FluxRichtmter);
                            }
                    }
            }
    }