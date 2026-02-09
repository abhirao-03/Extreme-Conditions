#include "simulation.h"

void Simulation::m_OutputToFile(std::ofstream& outputFile, const int& timestep, const double& time)
    {
        for (int j = m_iNumGhostCells / 2; j < m_iYNumPoints + m_iNumGhostCells / 2; j++)
            {
                for (int i = m_iNumGhostCells / 2; i < m_iXNumPoints + m_iNumGhostCells / 2; i++)
                    {
                        double x = m_dXStart + (i + 0.5) * m_dDeltaX;
                        double y = m_dYStart + (j + 0.5) * m_dDeltaY;
                        
                        vec4 U = m_vec_dU[j][i];
                        
                        outputFile << timestep << ","
                                << time << ","
                                << x << ","
                                << y << ","
                                << U[0] << ","  // density
                                << U[1] << ","  // x-momentum
                                << U[2] << ","  // y-momentum
                                << U[3]         // energy
                                << "\n";
                    }
            }
    }

void Simualtion::m_BilinearInterpolation();