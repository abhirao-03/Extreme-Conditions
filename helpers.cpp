#include "simulation.h"

vec4 Simulation::m_GetPrimitives(const vec4& f_vec3_U)
    {
        double density = f_vec3_U[0];
        double x_velocity = f_vec3_U[1] / density;
        double y_velocity = f_vec3_U[2] / density;
        
        double pressure = (m_dGamma - 1) * (f_vec3_U[3] - (1.0/2.0)*(density * (x_velocity*x_velocity + y_velocity*y_velocity)));

        return vec4(density, x_velocity, y_velocity, pressure);
    }

vec4 Simulation::m_GetConserved(const vec4& f_vec3_P)
{
    double density    = f_vec3_P[0];
    double x_velocity = f_vec3_P[1];
    double y_velocity = f_vec3_P[2];
    double pressure   = f_vec3_P[3];

    double x_momentum = density * x_velocity;
    double y_momentum = density * y_velocity;
    
    double energy = pressure / (m_dGamma - 1.0) + 0.5 * density * (x_velocity*x_velocity + y_velocity*y_velocity);

    return vec4(density, x_momentum, y_momentum, energy);
}

double Simulation::m_GetEnergy(const vec4& f_vec4_P)
    {
        double u_dVelocity = std::pow(f_vec4_P[1] * f_vec4_P[1] + f_vec4_P[2]*f_vec4_P[2], 0.5);
        return f_vec4_P[3]/(m_dGamma - 1.0) + (1.0/2.0) * (f_vec4_P[0]) * (pow(u_dVelocity, 2.0));
    }


vec4 Simulation::m_vXFlux(const vec4& f_vec4_U)
    {
        vec4 h_dPrimitve = m_GetPrimitives(f_vec4_U);
        double h_dEnergy = m_GetEnergy(h_dPrimitve);

        double& h_dDensity   = h_dPrimitve[0];
        double& h_dXVelocity = h_dPrimitve[1];
        double& h_dYVelocity = h_dPrimitve[2];
        double& h_dPressure  = h_dPrimitve[3];
        
        double h_dFirstTerm  = h_dDensity*h_dXVelocity;
        double h_dSecondTerm = h_dDensity*h_dXVelocity*h_dXVelocity + h_dPressure;
        double h_dThirdTerm  = h_dDensity*h_dXVelocity*h_dYVelocity;
        double h_dFourthTerm = (h_dEnergy + h_dPressure)*h_dXVelocity ;

        return vec4(h_dFirstTerm, h_dSecondTerm, h_dThirdTerm, h_dFourthTerm);
    };
                
vec4 Simulation::m_vYFlux(const vec4& f_vec4_U)
    {
        vec4 h_dPrimitve = m_GetPrimitives(f_vec4_U);
        double h_dEnergy = m_GetEnergy(h_dPrimitve);

        double& h_dDensity   = h_dPrimitve[0];
        double& h_dXVelocity = h_dPrimitve[1];
        double& h_dYVelocity = h_dPrimitve[2];
        double& h_dPressure  = h_dPrimitve[3];
        
        double h_dFirstTerm  = h_dDensity*h_dXVelocity;
        double h_dSecondTerm = h_dDensity*h_dXVelocity*h_dYVelocity;
        double h_dThirdTerm  = h_dDensity*h_dYVelocity*h_dYVelocity + h_dPressure;
        double h_dFourthTerm = (h_dEnergy + h_dPressure)*h_dYVelocity ;

        return vec4(h_dFirstTerm, h_dSecondTerm, h_dThirdTerm, h_dFourthTerm);
    };

void Simulation::SetBoundaryConditions()
{
    int ghost = m_iNumGhostCells / 2;
    
    for (int j = ghost; j < m_iYNumPoints + ghost; j++)
        {
            // Left boundary (x = xStart)
            for (int g = 0; g < ghost; g++)
                {
                    m_vec_dU[j][g] = m_vec_dU[j][ghost];
                }
                
            // Right boundary (x = xEnd)
            for (int g = 0; g < ghost; g++)
                {
                    m_vec_dU[j][m_iXNumPoints + ghost + g] = m_vec_dU[j][m_iXNumPoints + ghost - 1];  // Outflow
                }
        }
    
    for (int i = 0; i < m_iXNumPoints + m_iNumGhostCells; i++)
        {
            // Bottom boundary (y = yStart)
            for (int g = 0; g < ghost; g++)
                {
                    m_vec_dU[g][i] = m_vec_dU[ghost][i];
                }
            
            // Top boundary (y = yEnd)
            for (int g = 0; g < ghost; g++)
                {
                    m_vec_dU[m_iYNumPoints + ghost + g][i] = m_vec_dU[m_iYNumPoints + ghost - 1][i];
                }
        }
}

void Simulation::OutputToFile(std::ofstream& outputFile, const int& timestep, const double& time)
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
                                << std::endl;
                    }
            }
    }