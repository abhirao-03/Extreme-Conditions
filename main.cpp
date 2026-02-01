#include <iostream>
#include "simulation.h"

int main(){
    double u_dX0 = 0.0;
    double u_dX1 = 1.0;
    
    double u_dY0 = 0.0;
    double u_dY1 = 1.0;
    
    double u_dT0 = 0.0;
    double u_dTEND = 1.0;
    
    double u_dRELAXATION = 0.8;
    double u_dGAMMA = 1.4;

    int u_iXNUMPOINTS = 100;
    int u_iYNUMPOINTS = 100;
    int u_iNUMGHOST = 2;

    int u_iInitialCondition;
    std::cin >> u_iInitialCondition;
    
    Simulation::InitialCondition u_eInitialCondition = static_cast<Simulation::InitialCondition>(u_iInitialCondition);
    Simulation::SlopeLimiter u_eSlopeLimiter = Simulation::SlopeLimiter::MINBEE;
    
    Simulation TwoDimensionalEuler(u_dX0,
                                   u_dX1,
                                   u_dY0,
                                   u_dY1,
                                   u_dT0,
                                   u_dTEND,
                                   u_dRELAXATION,
                                   u_dGAMMA,
                                   u_iXNUMPOINTS,
                                   u_iYNUMPOINTS,
                                   u_iNUMGHOST,
                                   u_eInitialCondition,
                                   u_eSlopeLimiter);
    
    TwoDimensionalEuler.Evolve();
    
    return 0;
}