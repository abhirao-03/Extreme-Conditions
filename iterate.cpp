#include <iostream>
#include <vector>
#include "simulation.h"

void Simulation::SetTimeStep()
    {
        double f_dMaxInformationSpeed = 0.0;
        
        for (int i = 1; i < m_vec_dU.size() - 1; i++)
            {
                for (int j = 1; j < m_vec_dU[i].size() - 1; j++)
                    {
                        vec4 l_vec4_Primitive = m_GetPrimitives(m_vec_dU[i][j]);
                        
                        double l_dDensity  = l_vec4_Primitive[0];
                        double l_dXVelocity = l_vec4_Primitive[1];
                        double l_dYVelocity = l_vec4_Primitive[2];
                        double l_dPressure = l_vec4_Primitive[3];

                        double l_dSoundSpeed = std::sqrt(m_dGamma * l_dPressure / l_dDensity);

                        double l_dCurrentMax = std::max(std::abs(l_dXVelocity), std::abs(l_dYVelocity)) + l_dSoundSpeed;

                        if (l_dCurrentMax > f_dMaxInformationSpeed)
                            {
                                f_dMaxInformationSpeed = l_dCurrentMax;
                            }
                    };
            }

        if (f_dMaxInformationSpeed > 0.0)
            {
                m_dDeltaT = m_dRelaxation * m_dDeltaX / f_dMaxInformationSpeed;
            }
        else 
            {
                m_dDeltaT = 1e-4;
            }
    }
    
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
                for (int i = 0; i < m_iXNumPoints + m_iNumGhostCells; i++)
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
        double t = m_dTimeStart;
        int timestep = 0;
        int ghost = m_iNumGhostCells / 2;
        
        std::ofstream outputFile("./simulation_output.csv");
        outputFile << "timestep,time,x,y,density,x_momentum,y_momentum,energy" << std::endl;
        outputFile.precision(10);
        
        SetInitialCondition();
        SetLimitingFunction();
        SetBoundaryConditions();
        SetTimeStep();

        OutputToFile(outputFile, timestep, t);
            
        do
            {
                t += m_dDeltaT;
                timestep++;
                
                SetBoundaryConditions();
                
                m_ReconstructData(0);
                m_EvolveHalfTimeStep(0);
                m_GetReconstructedFluxes(0);
                for (int j = m_iNumGhostCells/2; j < m_iYNumPoints + m_iNumGhostCells/2; j++)
                    {
                        for (int i = m_iNumGhostCells/2; i < m_iXNumPoints + m_iNumGhostCells/2; i++)
                            {
                                m_vec_dUNext[j][i] = m_vec_dU[j][i] - (m_dDeltaT / m_dDeltaX) * (m_vec_dFluxesReconstructed[j][i] - m_vec_dFluxesReconstructed[j][i-1]);
                            }
                    }

                m_vec_dU = m_vec_dUNext;
                
                SetBoundaryConditions();
                
                m_ReconstructData(1);
                m_EvolveHalfTimeStep(1);
                m_GetReconstructedFluxes(1);
                for (int j = m_iNumGhostCells/2; j < m_iYNumPoints + m_iNumGhostCells/2; j++)
                    {
                        for (int i = m_iNumGhostCells/2; i < m_iXNumPoints + m_iNumGhostCells/2; i++)
                            {
                                m_vec_dUNext[j][i] = m_vec_dU[j][i] - (m_dDeltaT / m_dDeltaX) * (m_vec_dFluxesReconstructed[j][i] - m_vec_dFluxesReconstructed[j - 1][i]);
                            }
                    }
                
                m_vec_dU = m_vec_dUNext;
                    
                OutputToFile(outputFile, timestep, t);
                
                std::cout << "Time: " << t << " / " << m_dTimeEnd << "\n";
                
                SetTimeStep();

            } while (t < m_dTimeEnd);
        
        outputFile.close();
        std::cout << "\nSimulation complete! Output written to simulation_output.csv" << std::endl;
    }