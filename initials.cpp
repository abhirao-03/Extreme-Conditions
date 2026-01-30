#include "simulation.h"
static const double PI = 4*atan(1);

void Simulation::SetInitialCondition()
    {
        switch (m_eInitialCondition)
            {
                case InitialCondition::INITIAL_ONE:
                    InitialOne(m_vec_dU);
                    break;

                case InitialCondition::INITIAL_TWO:
                    InitialTwo(m_vec_dU);
                    break;

                case InitialCondition::INITIAL_THREE:
                    InitialThree(m_vec_dU);
                    break;

                case InitialCondition::INITIAL_FOUR:
                    InitialFour(m_vec_dU);
                    break;

                case InitialCondition::TORO_INIT_ONE:
                    m_dTimeEnd = 0.25;
                    ToroInitialOne(m_vec_dU);
                    break;

                case InitialCondition::TORO_INIT_TWO:
                    m_dTimeEnd = 0.15;
                    ToroInitialTwo(m_vec_dU);
                    break;

                case InitialCondition::TORO_INIT_THREE:
                    m_dTimeEnd = 0.012;
                    ToroInitialThree(m_vec_dU);
                    break;

                case InitialCondition::TORO_INIT_FOUR:
                    m_dTimeEnd = 0.035;
                    ToroInitialFour(m_vec_dU);
                    break;

                case InitialCondition::TORO_INIT_FIVE:
                    m_dTimeEnd = 0.035;
                    ToroInitialFive(m_vec_dU);
                    break;

                case InitialCondition::SINE_WAVE:
                    m_dTimeEnd = 1.0;
                    InitialSineWave(m_vec_dU);
                    break;
            }
    }

void Simulation::InitialOne(std::vector<std::vector<vec4>>& vec_dU)
    {
        double l_dDensity = 1.0;
        double l_dXVelocity = 0.0;
        double l_dYVelocity = 0.0;
        double l_dPressure = 1.0;

        double l_dEnergy = m_GetEnergy(vec4(l_dDensity, l_dXVelocity, l_dYVelocity, l_dPressure));

        for (std::vector<vec4>& l_vec4XDirection : vec_dU)
            {
                for (vec4& l_vec4ConservedVars : l_vec4XDirection)
                    {
                        l_vec4ConservedVars[0] = l_dDensity;
                        l_vec4ConservedVars[1] = l_dDensity*l_dXVelocity;
                        l_vec4ConservedVars[2] = l_dDensity*l_dYVelocity;
                        l_vec4ConservedVars[3] = l_dEnergy;
                    }
            }
    }

void Simulation::InitialTwo(std::vector<std::vector<vec4>>& vec_dU)
    {
        double l_dDensity = 2.0;
        double l_dXVelocity = 1.0;
        double l_dYVelocity = 0.0;
        double l_dPressure = 3.0;

        double l_dEnergy = m_GetEnergy(vec4(l_dDensity, l_dXVelocity, l_dYVelocity, l_dPressure));
        
        for (std::vector<vec4>& l_vec4XDirection : vec_dU)
            {
                for (vec4& l_vec4ConservedVars : l_vec4XDirection)
                    {
                        l_vec4ConservedVars[0] = l_dDensity;
                        l_vec4ConservedVars[1] = l_dDensity*l_dXVelocity;
                        l_vec4ConservedVars[2] = l_dDensity*l_dYVelocity;
                        l_vec4ConservedVars[3] = l_dEnergy;
                    }
            }
    }

void Simulation::InitialThree(std::vector<std::vector<vec4>>& vec_dU)
    {
        double l_dLeftDensity = 1.0;
        double l_dLeftXVelocity = 0.0;
        double l_dLeftYVelocity = 0.0;
        double l_dLeftPressure = 1.0;
        double l_dLeftEnergy = m_GetEnergy(vec4(l_dLeftDensity, l_dLeftXVelocity, l_dLeftYVelocity, l_dLeftPressure));

        double l_dRightDensity = 0.1;
        double l_dRightXVelocity = 0.0;
        double l_dRightYVelocity = 0.0;
        double l_dRightPressure = 1.0;
        double l_dRightEnergy = m_GetEnergy(vec4(l_dRightDensity, l_dRightXVelocity, l_dRightYVelocity, l_dRightPressure));

        double l_dDiscontinuityLocation = 0.5;

        for (std::vector<vec4>& l_vec4XDirection : vec_dU)
            {
                for (int i = 0; i < l_vec4XDirection.size(); i++)
                    {
                        vec4& l_vec4ConservedVars = l_vec4XDirection[i];
        
                        double l_dXVal = m_dXStart + m_dDeltaX * (i - 0.5);
        
                        if (l_dXVal <= l_dDiscontinuityLocation)
                            {
                                l_vec4ConservedVars[0] = l_dLeftDensity;
                                l_vec4ConservedVars[1] = l_dLeftDensity*l_dLeftXVelocity;
                                l_vec4ConservedVars[2] = l_dLeftDensity*l_dLeftYVelocity;
                                l_vec4ConservedVars[3] = l_dLeftEnergy;
                            } else {
                                l_vec4ConservedVars[0] = l_dRightDensity;
                                l_vec4ConservedVars[1] = l_dRightDensity*l_dRightXVelocity;
                                l_vec4ConservedVars[2] = l_dRightDensity*l_dRightYVelocity;
                                l_vec4ConservedVars[3] = l_dRightEnergy;
                            }
                    }
            }
    }

void Simulation::InitialFour(std::vector<std::vector<vec4>>& vec_dU)
    {
        double l_dLeftDensity = 1.0;
        double l_dLeftXVelocity = 1.0;
        double l_dLeftYVelocity = 0.0;
        double l_dLeftPressure = 1.0;
        double l_dLeftEnergy = m_GetEnergy(vec4(l_dLeftDensity, l_dLeftXVelocity, l_dLeftYVelocity, l_dLeftPressure));

        double l_dRightDensity = 0.1;
        double l_dRightXVelocity = 1.0;
        double l_dRightYVelocity = 0.0;
        double l_dRightPressure = 1.0;
        double l_dRightEnergy = m_GetEnergy(vec4(l_dRightDensity, l_dRightXVelocity, l_dRightYVelocity, l_dRightPressure));

        double l_dDiscontinuityLocation = 0.25;

        for (std::vector<vec4>& l_vec4XDirection : vec_dU)
            {
                /**
                * Note: I've changed below loop condition as it seemed wrong. InitialFour and ToroInitialOne...Five as well as
                * InitialSineWave used vec_dU.size() for the bound in the inner/X loop.. -> I changed it to use l_vec4XDirection.size()
                * since the latter would mean not all x cells get initialised? Experimentally, this seems to eliminate NaN's in affected
                * simulated cases
                * 
                * - Joseph
                */
                for (int i = 0; i < l_vec4XDirection.size(); i++)
                    {
                        vec4& l_vec4ConservedVars = l_vec4XDirection[i];

                        double l_dXVal = m_dXStart + m_dDeltaX * (i - 0.5);

                        if (l_dXVal <= l_dDiscontinuityLocation)
                            {
                                l_vec4ConservedVars[0] = l_dLeftDensity;
                                l_vec4ConservedVars[1] = l_dLeftDensity*l_dLeftXVelocity;
                                l_vec4ConservedVars[2] = l_dLeftDensity*l_dLeftYVelocity;
                                l_vec4ConservedVars[3] = l_dLeftEnergy;
                            } else {
                                l_vec4ConservedVars[0] = l_dRightDensity;
                                l_vec4ConservedVars[1] = l_dRightDensity*l_dRightXVelocity;
                                l_vec4ConservedVars[2] = l_dRightDensity*l_dRightYVelocity;
                                l_vec4ConservedVars[3] = l_dRightEnergy;
                            }
                    }
            }

    }

void Simulation::ToroInitialOne(std::vector<std::vector<vec4>>& vec_dU)
    {
        double l_dLeftDensity = 1.0;
        double l_dLeftXVelocity = 0.0;
        double l_dLeftYVelocity = 0.0;
        double l_dLeftPressure = 1.0;
        double l_dLeftEnergy = m_GetEnergy(vec4(l_dLeftDensity, l_dLeftXVelocity, l_dLeftYVelocity, l_dLeftPressure));

        double l_dRightDensity = 0.125;
        double l_dRightXVelocity = 0.0;
        double l_dRightYVelocity = 0.0;
        double l_dRightPressure = 0.1;
        double l_dRightEnergy = m_GetEnergy(vec4(l_dRightDensity, l_dRightXVelocity, l_dRightYVelocity, l_dRightPressure));

        double l_dDiscontinuityLocation = 0.5;

        for (std::vector<vec4>& l_vec4XDirection : vec_dU)
            {
                for (int i = 0; i < l_vec4XDirection.size(); i++)
                    {
                        vec4& l_vec4ConservedVars = l_vec4XDirection[i];
        
                        double l_dXVal = m_dXStart + m_dDeltaX * (i - 0.5);
        
                        if (l_dXVal <= l_dDiscontinuityLocation)
                            {
                                l_vec4ConservedVars[0] = l_dLeftDensity;
                                l_vec4ConservedVars[1] = l_dLeftDensity*l_dLeftXVelocity;
                                l_vec4ConservedVars[2] = l_dLeftDensity*l_dLeftYVelocity;
                                l_vec4ConservedVars[3] = l_dLeftEnergy;
                            } else {
                                l_vec4ConservedVars[0] = l_dRightDensity;
                                l_vec4ConservedVars[1] = l_dRightDensity*l_dRightXVelocity;
                                l_vec4ConservedVars[2] = l_dRightDensity*l_dRightYVelocity;
                                l_vec4ConservedVars[3] = l_dRightEnergy;
                            }
                    }
            }
    }


void Simulation::ToroInitialTwo(std::vector<std::vector<vec4>>& vec_dU)
    {
        double l_dLeftDensity = 1.0;
        double l_dLeftXVelocity = -2.0;
        double l_dLeftYVelocity = 0.0;
        double l_dLeftPressure = 0.4;
        double l_dLeftEnergy = m_GetEnergy(vec4(l_dLeftDensity, l_dLeftXVelocity, l_dLeftYVelocity, l_dLeftPressure));

        double l_dRightDensity = 1.0;
        double l_dRightXVelocity = 2.0;
        double l_dRightYVelocity = 0.0;
        double l_dRightPressure = 0.4;
        double l_dRightEnergy = m_GetEnergy(vec4(l_dRightDensity, l_dRightXVelocity, l_dRightYVelocity, l_dRightPressure));

        double l_dDiscontinuityLocation = 0.5;
        for (std::vector<vec4>& l_vec4XDirection : vec_dU)
            {
                for (int i = 0; i < l_vec4XDirection.size(); i++)
                    {
                        vec4& l_vec4ConservedVars = l_vec4XDirection[i];
        
                        double l_dXVal = m_dXStart + m_dDeltaX * (i - 0.5);
        
                        if (l_dXVal <= l_dDiscontinuityLocation)
                            {
                                l_vec4ConservedVars[0] = l_dLeftDensity;
                                l_vec4ConservedVars[1] = l_dLeftDensity*l_dLeftXVelocity;
                                l_vec4ConservedVars[2] = l_dLeftDensity*l_dLeftYVelocity;
                                l_vec4ConservedVars[3] = l_dLeftEnergy;
                            }
                        else
                            {
                                l_vec4ConservedVars[0] = l_dRightDensity;
                                l_vec4ConservedVars[1] = l_dRightDensity*l_dRightXVelocity;
                                l_vec4ConservedVars[2] = l_dRightDensity*l_dRightYVelocity;
                                l_vec4ConservedVars[3] = l_dRightEnergy;
                            }
                    }
            }
    }

void Simulation::ToroInitialThree(std::vector<std::vector<vec4>>& vec_dU)
    {
        double l_dLeftDensity = 1.0;
        double l_dLeftXVelocity = 0.0;
        double l_dLeftYVelocity = 0.0;
        double l_dLeftPressure = 1000.0;
        double l_dLeftEnergy = m_GetEnergy(vec4(l_dLeftDensity, l_dLeftXVelocity, l_dLeftYVelocity, l_dLeftPressure));

        double l_dRightDensity = 1.0;
        double l_dRightXVelocity = 0.0;
        double l_dRightYVelocity = 0.0;
        double l_dRightPressure = 0.01;
        double l_dRightEnergy = m_GetEnergy(vec4(l_dRightDensity, l_dRightXVelocity, l_dRightYVelocity, l_dRightPressure));

        double l_dDiscontinuityLocation = 0.5;
        for (std::vector<vec4>& l_vec4XDirection : vec_dU)
            {
                for (int i = 0; i < l_vec4XDirection.size(); i++)
                    {
                        vec4& l_vec4ConservedVars = l_vec4XDirection[i];
        
                        double l_dXVal = m_dXStart + m_dDeltaX * (i - 0.5);
        
                        if (l_dXVal <= l_dDiscontinuityLocation)
                            {
                                l_vec4ConservedVars[0] = l_dLeftDensity;
                                l_vec4ConservedVars[1] = l_dLeftDensity*l_dLeftXVelocity;
                                l_vec4ConservedVars[2] = l_dLeftDensity*l_dLeftYVelocity;
                                l_vec4ConservedVars[3] = l_dLeftEnergy;
                            }
                        else
                            {
                                l_vec4ConservedVars[0] = l_dRightDensity;
                                l_vec4ConservedVars[1] = l_dRightDensity*l_dRightXVelocity;
                                l_vec4ConservedVars[2] = l_dRightDensity*l_dRightYVelocity;
                                l_vec4ConservedVars[3] = l_dRightEnergy;
                            }
                    }
            }
    }

void Simulation::ToroInitialFour(std::vector<std::vector<vec4>>& vec_dU)
    {
        double l_dLeftDensity = 1.0;
        double l_dLeftXVelocity = 0.0;
        double l_dLeftYVelocity = 0.0;
        double l_dLeftPressure = 0.01;
        double l_dLeftEnergy = m_GetEnergy(vec4(l_dLeftDensity, l_dLeftXVelocity, l_dLeftYVelocity, l_dLeftPressure));

        double l_dRightDensity = 1.0;
        double l_dRightXVelocity = 0.0;
        double l_dRightYVelocity = 0.0;
        double l_dRightPressure = 100.0;
        double l_dRightEnergy = m_GetEnergy(vec4(l_dRightDensity, l_dRightXVelocity, l_dRightYVelocity, l_dRightPressure));

        double l_dDiscontinuityLocation = 0.5;
        
        for (std::vector<vec4>& l_vec4XDirection : vec_dU)
                    {
                        for (int i = 0; i < l_vec4XDirection.size(); i++)
                            {
                                vec4& l_vec4ConservedVars = l_vec4XDirection[i];
                
                                double l_dXVal = m_dXStart + m_dDeltaX * (i - 0.5);
                
                                if (l_dXVal <= l_dDiscontinuityLocation)
                                    {
                                        l_vec4ConservedVars[0] = l_dLeftDensity;
                                        l_vec4ConservedVars[1] = l_dLeftDensity*l_dLeftXVelocity;
                                        l_vec4ConservedVars[2] = l_dLeftDensity*l_dLeftYVelocity;
                                        l_vec4ConservedVars[3] = l_dLeftEnergy;
                                    }
                                else
                                    {
                                        l_vec4ConservedVars[0] = l_dRightDensity;
                                        l_vec4ConservedVars[1] = l_dRightDensity*l_dRightXVelocity;
                                        l_vec4ConservedVars[2] = l_dRightDensity*l_dRightYVelocity;
                                        l_vec4ConservedVars[3] = l_dRightEnergy;
                                    }
                            }
                    }
    }

void Simulation::ToroInitialFive(std::vector<std::vector<vec4>>& vec_dU)
    {
        double l_dLeftDensity = 5.99924;
        double l_dLeftXVelocity = 19.5975;
        double l_dLeftYVelocity = 0;
        double l_dLeftPressure = 460.894;
        double l_dLeftEnergy = m_GetEnergy(vec4(l_dLeftDensity, l_dLeftXVelocity, l_dLeftYVelocity, l_dLeftPressure));

        double l_dRightDensity = 5.99242;
        double l_dRightXVelocity = -6.19633;
        double l_dRightYVelocity = 0;
        double l_dRightPressure = 46.0950;
        double l_dRightEnergy = m_GetEnergy(vec4(l_dRightDensity, l_dRightXVelocity, l_dRightYVelocity, l_dRightPressure));

        double l_dDiscontinuityLocation = 0.5;

        for (std::vector<vec4>& l_vec4XDirection : vec_dU)
            {
                for (int i = 0; i < l_vec4XDirection.size(); i++)
                    {
                        vec4& l_vec4ConservedVars = l_vec4XDirection[i];
        
                        double l_dXVal = m_dXStart + m_dDeltaX * (i - 0.5);
        
                        if (l_dXVal <= l_dDiscontinuityLocation)
                            {
                                l_vec4ConservedVars[0] = l_dLeftDensity;
                                l_vec4ConservedVars[1] = l_dLeftDensity*l_dLeftXVelocity;
                                l_vec4ConservedVars[2] = l_dLeftDensity*l_dLeftYVelocity;
                                l_vec4ConservedVars[3] = l_dLeftEnergy;
                            }
                        else
                            {
                                l_vec4ConservedVars[0] = l_dRightDensity;
                                l_vec4ConservedVars[1] = l_dRightDensity*l_dRightXVelocity;
                                l_vec4ConservedVars[2] = l_dRightDensity*l_dRightYVelocity;
                                l_vec4ConservedVars[3] = l_dRightEnergy;
                            }
                    }
            }
    }

void Simulation::InitialSineWave(std::vector<std::vector<vec4>>& vec_dU)
{
    // Parameters
    double l_dXVelocity = 1.0;
    double l_dYVelocity = 0.0;
    double l_dPressure = 1.0;

    for (std::vector<vec4>& l_vec4XDirection : vec_dU)
        {
            for (int i = 0; i < l_vec4XDirection.size(); i++)
                {
                    vec4& l_vec4ConservedVars = l_vec4XDirection[i];
            
                    // Calculate cell center (Account for ghost cells!)
                    // If i=0 is ghost, x should be -0.5*dx. 
                    double l_dXVal = m_dXStart + (i - 0.5) * m_dDeltaX;
            
                    // Density Sine Wave
                    double l_dDensity = 1.0 + 0.2 * sin(2.0 * PI * l_dXVal);
            
                    // Calculate Energy
                    double l_dEnergy = m_GetEnergy(vec4(l_dDensity, l_dXVelocity, l_dYVelocity, l_dPressure));
            
                    // Set Conserved Variables
                    l_vec4ConservedVars[0] = l_dDensity;
                    l_vec4ConservedVars[1] = l_dDensity * l_dXVelocity;
                    l_vec4ConservedVars[2] = l_dDensity * l_dYVelocity;
                    l_vec4ConservedVars[3] = l_dEnergy;
                }
        }
}