#include <iostream>
#include "simulation.h"

void Simulation::m_EvolveHalfTimeStep(int f_iDirection)
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
    
        for (int j = 0; j < m_iYNumPoints + m_iNumGhostCells; j++)
            {
                for (int i = 0; m_iXNumPoints + m_iNumGhostCells; i++)
                    {
                        if (f_iDirection == 0)
                            {
                                vec4 t_dULeft = m_vec_LeftReconstructed[j][i];
                                vec4 t_dURight = m_vec_RightReconstructed[j][i];
                                
                                vec4 t_dFluxLeft  = (this->*t_CurrentFluxFunction)(t_dULeft);
                                vec4 t_dFluxRight = (this->*t_CurrentFluxFunction)(t_dURight);
                                
                                vec4 l_vdUpdatePortion = (0.5 * m_dDeltaT / t_dDeltaDirection) * (t_dFluxLeft - t_dFluxRight);
                                m_vec_LeftReconstructed[j][i] += l_vdUpdatePortion;
                                m_vec_RightReconstructed[j][i] += l_vdUpdatePortion;
                            }
                        else 
                            {
                                vec4 t_dULeft = m_vec_LeftReconstructed[j][i];
                                vec4 t_dURight = m_vec_RightReconstructed[j][i];
                                
                                vec4 t_dFluxLeft  = (this->*t_CurrentFluxFunction)(t_dULeft);
                                vec4 t_dFluxRight = (this->*t_CurrentFluxFunction)(t_dURight);
                                
                                vec4 l_vdUpdatePortion = (0.5 * m_dDeltaT / t_dDeltaDirection) * (t_dFluxLeft - t_dFluxRight);
                                m_vec_LeftReconstructed[j][i] += l_vdUpdatePortion;
                                m_vec_RightReconstructed[j][i] += l_vdUpdatePortion;
                            }
                    }
            }
    }
    
void Simulation::Evolve()
    {
        SetInitialCondition();
        SetLimitingFunction();
        SetBoundaryConditions();
        SetTimeStep();
        
        double t = m_dTimeStart;
        
        do
            {
                t += m_dDeltaT;
                
                SetBoundaryConditions();
                m_ReconstructData();
                m_EvolveHalfTimeStep();
                m_GetReconstructedFluxes();
                
                std::cout << "# time = " << t << std::endl;

                for (int i = 1; i < m_vec_dU.size() - 1; i++)
                    {
                        double x = m_dXStart + (i - m_iNumGhostCells + 0.5) * m_dDeltaX;
                        
                        m_vec_dUNext[i] = m_vec_dU[i] - (m_dDeltaT / m_dDeltaX) * (m_vec_dFluxesReconstructed[i] - m_vec_dFluxesReconstructed[i-1]);
                        
                        std::cout << x << ' ' << m_vec_dUNext[i] << std::endl;
                    }

                std::cout << "\n\n";

                m_vec_dU = m_vec_dUNext;
                
                SetTimeStep();

            } while (t < m_dTimeEnd);
    }