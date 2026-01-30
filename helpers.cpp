#include <iostream>
#include "simulation.h"

void Simulation::m_GetU()
    {
        for (int i = 0; i < m_vec_dU.size(); i++)
            {
                double x = m_dXStart + (i - 0.5) * m_dDeltaX;
                std::cout << x << ' ' << m_vec_dU[i] << std::endl;
            }
    }

vec4 Simulation::m_GetPrimitives(const vec4& f_vec3_U)
    {

        double density = f_vec3_U[0];
        double x_velocity = f_vec3_U[1] / density;
        double y_velocity = f_vec3_U[2] / density;
        
        double velocity = std::pow(x_velocity*x_velocity + y_velocity*y_velocity, 0.5);
        
        double pressure = (m_dGamma - 1) * (f_vec3_U[2] - (1.0/2.0)*(density * (pow(velocity, 2.0))));

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
    
    double velocity = std::pow(x_velocity*x_velocity + y_velocity*y_velocity, 0.5);
    
    double energy = pressure / (m_dGamma - 1.0) + 0.5 * density * velocity*velocity;

    return vec4(density, x_momentum, y_momentum, energy);
}

double Simulation::m_GetEnergy(const vec4& f_vec4_P)
    {
        double u_dVelocity = std::pow(f_vec4_P[1] * f_vec4_P[1] + f_vec4_P[2]*f_vec4_P[2], 0.5);
        return f_vec4_P[3]/(m_dGamma - 1.0) + (1.0/2.0) * (f_vec4_P[0]) * (pow(u_dVelocity, 2.0));
    }
