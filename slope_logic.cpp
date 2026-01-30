#include "simulation.h"

vec4 Simulation::m_GetSlopeMeasure(const int& t_iCellValue)
    {
        // Handle first and last cell values;
        if (t_iCellValue == 0)
            {
                return vec4(0.0, 0.0, 0.0, 0.0);
            }

        else if (t_iCellValue == m_vec_dU.size() - 1)
            {
                return vec4(0.0, 0.0, 0.0, 0.0);
            }

        // Handle the interiors.
        else
            {
                vec4 t_vec3Current = m_vec_dU[t_iCellValue];
                vec4 t_vec3Previous = m_vec_dU[t_iCellValue - 1];
                vec4 t_vec3Future = m_vec_dU[t_iCellValue + 1];

                vec4 t_vec3FirstTerm = (1.0/2.0) * (1 + m_dOmega) * (t_vec3Current - t_vec3Previous);
                vec4 t_vec3SecondTerm = (1.0/2.0) * (1 - m_dOmega) * (t_vec3Future - t_vec3Current);

                return t_vec3FirstTerm + t_vec3SecondTerm;
            }
    }

void Simulation::m_ReconstructData()
    {
        std::vector<vec4> t_vConserved = m_vec_dU;
        
        for (int i = 0; i < m_vec_dU.size(); i++)
            {
                m_vec_dU[i] = m_GetPrimitives(t_vConserved[i]);
            }
        
        for(int i = 0; i < m_vec_dU.size(); i++)
            {
                m_vec_LeftReconstructed[i]  = m_vec_dU[i];
                m_vec_RightReconstructed[i] = m_vec_dU[i];
            }


        for (int i = 1; i < m_vec_dU.size() - 1; i++)
            {
                vec4 l_vDeltaI  = m_GetSlopeMeasure(i);
                vec4 l_vLimiter = (this->*m_LimitingFunction)(i);
                
                m_vec_LeftReconstructed[i]  = m_vec_dU[i] - (1.0/2.0)*l_vLimiter*l_vDeltaI;
                m_vec_RightReconstructed[i] = m_vec_dU[i] + (1.0/2.0)*l_vLimiter*l_vDeltaI;
            }
        
        for (int i = 0; i < m_vec_dU.size(); i++)
            {
                m_vec_LeftReconstructed[i]  = m_GetConserved(m_vec_LeftReconstructed[i]);
                m_vec_RightReconstructed[i] = m_GetConserved(m_vec_RightReconstructed[i]);
            }
        
        m_vec_dU = t_vConserved;
    }
    
void Simulation::m_GetReconstructedFluxes()
    {
        FluxFunction t_CurrentFluxFunction;
        
        for (int i = 0; i < m_vec_dU.size() - 1; i++)
            {
                vec4 t_dULeft  = m_vec_RightReconstructed[i];
                vec4 t_dURight = m_vec_LeftReconstructed[i+1];
                
                vec4 t_dFluxLeft  = m_EulerFluxFunction(t_dULeft);
                vec4 t_dFluxRight = m_EulerFluxFunction(t_dURight);
                
                //LaxFriedrich Flux Get
                vec4 FluxLaxFriedrichs = 0.5 * (t_dFluxLeft + t_dFluxRight) + 0.5 * (m_dDeltaX/m_dDeltaT) * (t_dULeft - t_dURight);
                
                //Richtmyer Flux get
                // Step 1 LF Update;
                vec4 U_Richtmyer = 0.5 * (t_dULeft + t_dURight) + 0.5 * (m_dDeltaT / m_dDeltaX) * (t_dFluxLeft - t_dFluxRight);
                vec4 FluxRichtmter = m_EulerFluxFunction(U_Richtmyer);
                
                m_vec_dFluxesReconstructed[i] = 0.5 * (FluxLaxFriedrichs + FluxRichtmter);
            }
    }