#include <cmath>
#include <fstream>
#include <vector>
#include "EulerVectors.h"
static const double PI = 4*atan(1);

class Simulation
{
    private:
        double m_dXStart;
        double m_dXEnd;
        double m_dYStart;
        double m_dYEnd;
        double m_dTimeStart;
        double m_dTimeEnd;

        int m_iXNumPoints;
        int m_iYNumPoints;
        int m_iNumGhostCells;

        double m_dDeltaX;
        double m_dDeltaY;
        double m_dRelaxation;
        double m_dDeltaT;

        double m_dGamma = 1.4;
        double m_dOmega = 0.0;

        std::vector<vec4> m_vec_dU;
        std::vector<vec4> m_vec_dFluxes;

        std::vector<vec4> m_vec_LeftReconstructed;
        std::vector<vec4> m_vec_RightReconstructed;
        std::vector<vec4> m_vec_dFluxesReconstructed;
    
        std::vector<vec4> m_vec_dUNext;

        using LimitingFunction = vec4 (Simulation::*)(const int& l_iIterValue);
        LimitingFunction m_LimitingFunction = nullptr;
        
        using FluxFunction = vec4 (Simulation::*)(const vec4&);
        FluxFunction m_FluxFunction = nullptr;
        
        public:
            enum class InitialCondition
                {
                    INITIAL_ONE = 1,
                    INITIAL_TWO = 2,
                    INITIAL_THREE = 3,
                    INITIAL_FOUR = 4,
                    TORO_INIT_ONE = 5,
                    TORO_INIT_TWO = 6,
                    TORO_INIT_THREE = 7,
                    TORO_INIT_FOUR = 8,
                    TORO_INIT_FIVE = 9,
                    SINE_WAVE = 10
                };
                
            enum SlopeLimiter
                {
                    SUPERBEE = 1,
                    VAN_LEER = 2,
                    VAN_ALBADA = 3,
                    MINBEE = 4
                };
    
            InitialCondition m_eInitialCondition;
            SlopeLimiter m_eSlopeLimiter;
            
    
            // member initialization
            Simulation(
                        double dxStart,
                        double dxEnd,
                        double dyStart,
                        double dyEnd,
                        double dTimeStart,
                        double dTimeEnd,
                        double dRelaxation,
                        double dGamma,
                        int iXNumPoints,
                        int iYNumPoints,
                        int iNumGhostCells,
                        InitialCondition eInitialCondition,
                        SlopeLimiter eSlopeLimiter
                    )
                :
                m_dXStart(dxStart),
                m_dXEnd(dxEnd),
                m_dYStart(dyStart),
                m_dYEnd(dyEnd),
                m_dTimeStart(dTimeStart),
                m_dTimeEnd(dTimeEnd),
                m_dRelaxation(dRelaxation),
                m_dGamma(dGamma),
                m_iXNumPoints(iXNumPoints),
                m_iYNumPoints(iYNumPoints),
                m_iNumGhostCells(iNumGhostCells),
                m_eInitialCondition(eInitialCondition),
                m_eSlopeLimiter(eSlopeLimiter)
                {
                    m_dDeltaX = (m_dXEnd - m_dXStart) / m_iXNumPoints;
                    m_dDeltaT = m_dRelaxation * m_dDeltaX;
    
                    m_vec_dU.resize(2*m_iNumGhostCells + m_iXNumPoints*m_iYNumPoints);
                    m_vec_dUNext.resize(2*m_iNumGhostCells + m_iXNumPoints*m_iYNumPoints);
                    
                    m_vec_dFluxes.resize(2*m_iNumGhostCells + m_iXNumPoints*m_iYNumPoints);
                    m_vec_dFluxesReconstructed.resize(2*m_iNumGhostCells + m_iXNumPoints*m_iYNumPoints);
    
                    m_vec_LeftReconstructed.resize(2*m_iNumGhostCells + m_iXNumPoints*m_iYNumPoints);
                    m_vec_RightReconstructed.resize(2*m_iNumGhostCells + m_iXNumPoints*m_iYNumPoints);
                }
    
            void InitialOne(std::vector<vec4>& vec_dU);
            void InitialTwo(std::vector<vec4>& vec_dU);
            void InitialThree(std::vector<vec4>& vec_dU);
            void InitialFour(std::vector<vec4>& vec_dU);
    
            void ToroInitialOne(std::vector<vec4>& vec_dU);
            void ToroInitialTwo(std::vector<vec4>& vec_dU);
            void ToroInitialThree(std::vector<vec4>& vec_dU);
            void ToroInitialFour(std::vector<vec4>& vec_dU);
            void ToroInitialFive(std::vector<vec4>& vec_dU);
    
            void InitialSineWave(std::vector<vec4>& vec_dU);
    
            void m_fvm_LaxFriedrichs(std::vector<vec4>& vec_dInputVector, std::vector<vec4>& vec_dUpdateVector);
            void m_fvm_Richtmyer(std::vector<vec4>& vec_dInputVector, std::vector<vec4>& vec_dUpdateVector);
            void m_fvm_FORCE(std::vector<vec4>& vec_dInputVector, std::vector<vec4>& vec_dUpdateVector);
    
            vec4 m_SL_Superbee(const int& l_iIterValue);
            vec4 m_SL_VanLeer(const int& l_iIterValue);
            vec4 m_SL_VanAlbada(const int& l_iIterValue);
            vec4 m_SL_Minbee(const int& l_iIterValue);
    
    
            void m_GetU();
            double m_GetEnergy(const vec4& f_vec4_P);
    
            vec4 m_GetPrimitives(const vec4& f_vec4_U);
            vec4 m_GetConserved(const vec4& f_vec4_P);
            vec4 m_GetSlopeLimitingR(const int& l_iIterValue);
            vec4 m_GetSlopeMeasure(const int& t_iCellValue);
    
            void m_ReconstructData();
            void m_GetReconstructedFluxes();
            void m_EvolveHalfTimeStep();
            
            vec4 m_vXFlux(const vec4& f_vec4_U)
                {
                    vec4 prims = m_GetPrimitives(f_vec4_U);
                    double h_dEnergy = m_GetEnergy(prims);
    
                    double& h_dDensity   = prims[0];
                    double& h_dXVelocity = prims[1];
                    double& h_dYVelocity = prims[2];
                    double& h_dPressure  = prims[3];
                    
                    double h_dFirstTerm  = h_dDensity*h_dXVelocity;
                    double h_dSecondTerm = h_dDensity*h_dXVelocity*h_dXVelocity + h_dPressure;
                    double h_dThirdTerm  = h_dDensity*h_dXVelocity*h_dYVelocity;
                    double h_dFourthTerm = (h_dEnergy + h_dPressure)*h_dXVelocity ;
    
                    return vec4();
                };
                
            vec4 m_vYFlux(const vec4& f_vec4_U)
                {
                    vec4 prims = m_GetPrimitives(f_vec4_U);
                    double h_dEnergy = m_GetEnergy(prims);
    
                    double& h_dDensity   = prims[0];
                    double& h_dXVelocity = prims[1];
                    double& h_dYVelocity = prims[2];
                    double& h_dPressure  = prims[3];
                    
                    double h_dFirstTerm  = h_dDensity*h_dXVelocity;
                    double h_dSecondTerm = h_dDensity*h_dXVelocity*h_dYVelocity;
                    double h_dThirdTerm  = h_dDensity*h_dYVelocity*h_dYVelocity + h_dPressure;
                    double h_dFourthTerm = (h_dEnergy + h_dPressure)*h_dYVelocity ;
    
                    return vec4(h_dFirstTerm, h_dSecondTerm, h_dThirdTerm, h_dFourthTerm);
                };
    
            void SetTimeStep()
                {
                    double f_dMaxInformationSpeed = 0.0;
    
                    for (int i = 1; i < m_vec_dU.size() - 1; i++)
                        {
                            vec4 l_vec4_Primitive = m_GetPrimitives(m_vec_dU[i]);
                            double l_dDensity  = l_vec4_Primitive[0];
                            double l_dVelocity = l_vec4_Primitive[1];
                            double l_dPressure = l_vec4_Primitive[2];
    
                            double l_dSoundSpeed = std::sqrt(m_dGamma * l_dPressure / l_dDensity);
    
                            double l_dCurrentMax = std::abs(l_dVelocity) + l_dSoundSpeed;
    
                            if (l_dCurrentMax > f_dMaxInformationSpeed)
                                {
                                    f_dMaxInformationSpeed = l_dCurrentMax;
                                }
                        };
    
                    if (f_dMaxInformationSpeed > 0.0)
                    {
                        m_dDeltaT = m_dRelaxation * m_dDeltaX / f_dMaxInformationSpeed;
                    } else  {
                        m_dDeltaT = 1e-4;
                    }
                }
    
            void SetInitialCondition()
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
    
            void SetLimitingFunction()
                {
                    switch (m_eSlopeLimiter)
                        {
                            case SlopeLimiter::SUPERBEE:
                                m_LimitingFunction = &Simulation::m_SL_Superbee;
                                break;
    
                            case SlopeLimiter::VAN_LEER:
                                m_LimitingFunction = &Simulation::m_SL_VanLeer;
                                break;
    
                            case SlopeLimiter::VAN_ALBADA:
                                m_LimitingFunction = &Simulation::m_SL_VanAlbada;
                                break;
    
                            case SlopeLimiter::MINBEE:
                                m_LimitingFunction = &Simulation::m_SL_Minbee;
                                break;
    
                            default:
                                m_LimitingFunction = &Simulation::m_SL_Minbee;
                                break;
                        }
                }
    
            void SetBoundaryConditions();
            void Evolve();
};
