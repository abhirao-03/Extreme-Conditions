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
                
                case InitialCondition::SPHERE_BLAST:
                    m_dTimeEnd = 0.2;
                    InitialSphere(m_vec_dU);
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

void Simulation::InitialSphere(std::vector<std::vector<vec4>>& vec_dU)
{
    // 1. Define States
    double l_dInDensity   = 1.0;
    double l_dInPressure  = 1.0;
    double l_dInEnergy    = m_GetEnergy(vec4(l_dInDensity, 0.0, 0.0, l_dInPressure));

    double l_dOutDensity  = 0.125;
    double l_dOutPressure = 0.1;
    double l_dOutEnergy   = m_GetEnergy(vec4(l_dOutDensity, 0.0, 0.0, l_dOutPressure));

    // 2. Geometry Calculation
    double l_dWidth  = m_dXEnd - m_dXStart;
    double l_dHeight = m_dYEnd - m_dYStart;
    
    double l_dCenterX = m_dXStart + l_dWidth / 2.0;
    double l_dCenterY = m_dYStart + l_dHeight / 2.0;
    
    // Radius: 20% of the smaller dimension to ensure it fits
    double l_dRadius  = std::min(l_dWidth, l_dHeight) * 0.2;
    double l_dRadiusSq = l_dRadius * l_dRadius;

    int l_iGhostOffset = m_iNumGhostCells / 2;
    int l_iCellsInitialized = 0; // Counter to verify we actually draw the circle

    // DEBUG OUTPUT
    std::cout << "--- Initializing Sphere ---" << std::endl;
    std::cout << "Domain: X[" << m_dXStart << ", " << m_dXEnd << "] Y[" << m_dYStart << ", " << m_dYEnd << "]" << std::endl;
    std::cout << "Sphere: Center(" << l_dCenterX << ", " << l_dCenterY << ") Radius: " << l_dRadius << std::endl;

    for (int j = 0; j < vec_dU.size(); j++)
    {
        for (int i = 0; i < vec_dU[j].size(); i++)
        {
            // Calculate Physical Coordinates
            // Note: Indices match Evolve() logic
            double x = m_dXStart + (i - l_iGhostOffset) * m_dDeltaX + 0.5 * m_dDeltaX;
            double y = m_dYStart + (j - l_iGhostOffset) * m_dDeltaY + 0.5 * m_dDeltaY;

            // Distance Check
            double l_dDistSq = (x - l_dCenterX)*(x - l_dCenterX) + (y - l_dCenterY)*(y - l_dCenterY);

            vec4& l_vec4ConservedVars = vec_dU[j][i];

            if (l_dDistSq <= l_dRadiusSq)
            {
                // INSIDE
                l_vec4ConservedVars[0] = l_dInDensity;
                l_vec4ConservedVars[1] = 0.0;
                l_vec4ConservedVars[2] = 0.0;
                l_vec4ConservedVars[3] = l_dInEnergy;
                l_iCellsInitialized++;
            }
            else
            {
                // OUTSIDE
                l_vec4ConservedVars[0] = l_dOutDensity;
                l_vec4ConservedVars[1] = 0.0;
                l_vec4ConservedVars[2] = 0.0;
                l_vec4ConservedVars[3] = l_dOutEnergy;
            }
        }
    }
    
    std::cout << "Cells inside sphere: " << l_iCellsInitialized << std::endl;
    
    if (l_iCellsInitialized == 0)
    {
        std::cerr << "ERROR: Sphere radius is too small or coordinates are misaligned. No cells were initialized inside!" << std::endl;
    }
}