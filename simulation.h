#include <cmath>
#include <fstream>
#include <vector>
#include "EulerVectors.h"

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

        std::vector<std::vector<vec4>> m_vec_dU;
        std::vector<std::vector<vec4>> m_vec_dUNext;
        
        std::vector<std::vector<vec4>> m_vec_dFluxes;

        std::vector<std::vector<vec4>> m_vec_LeftReconstructed;
        std::vector<std::vector<vec4>> m_vec_RightReconstructed;
        std::vector<std::vector<vec4>> m_vec_dFluxesReconstructed;

        using LimitingFunction = vec4 (Simulation::*)(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right);
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
                    SINE_WAVE = 10,
                    SPHERE_BLAST = 11,
                };
                
            enum class SlopeLimiter
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
                    m_dDeltaY = (m_dYEnd - m_dYStart) / m_iYNumPoints;
                    m_dDeltaT = m_dRelaxation * m_dDeltaX;
    
                    m_vec_dU.resize(m_iNumGhostCells + m_iYNumPoints);
                    m_vec_dUNext.resize(m_iNumGhostCells + m_iYNumPoints);
                    
                    m_vec_LeftReconstructed.resize(m_iNumGhostCells + m_iYNumPoints);
                    m_vec_RightReconstructed.resize(m_iNumGhostCells + m_iYNumPoints);
                    
                    m_vec_dFluxes.resize(m_iNumGhostCells + m_iYNumPoints);
                    m_vec_dFluxesReconstructed.resize(m_iNumGhostCells + m_iYNumPoints);
                    
                    for (int i = 0; i < m_iNumGhostCells + m_iYNumPoints; i++)
                        {
                            m_vec_dU[i].resize(m_iNumGhostCells + m_iXNumPoints);
                            m_vec_dUNext[i].resize(m_iNumGhostCells + m_iXNumPoints);
                            
                            m_vec_LeftReconstructed[i].resize(m_iNumGhostCells + m_iXNumPoints);
                            m_vec_RightReconstructed[i].resize(m_iNumGhostCells + m_iXNumPoints);
                            
                            m_vec_dFluxes[i].resize(m_iNumGhostCells + m_iXNumPoints);
                            m_vec_dFluxesReconstructed[i].resize(m_iNumGhostCells + m_iXNumPoints);
                        }
                }
    
            void InitialOne(std::vector<std::vector<vec4>>& vec_dU);
            void InitialTwo(std::vector<std::vector<vec4>>& vec_dU);
            void InitialThree(std::vector<std::vector<vec4>>& vec_dU);
            void InitialFour(std::vector<std::vector<vec4>>& vec_dU);
            void ToroInitialOne(std::vector<std::vector<vec4>>& vec_dU);
            void ToroInitialTwo(std::vector<std::vector<vec4>>& vec_dU);
            void ToroInitialThree(std::vector<std::vector<vec4>>& vec_dU);
            void ToroInitialFour(std::vector<std::vector<vec4>>& vec_dU);
            void ToroInitialFive(std::vector<std::vector<vec4>>& vec_dU);
            void InitialSineWave(std::vector<std::vector<vec4>>& vec_dU);
            void InitialSphere(std::vector<std::vector<vec4>>& vec_dU);
            void m_SetInitialCondition();
    
            vec4 m_SL_Superbee(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right);
            vec4 m_SL_VanLeer(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right);
            vec4 m_SL_VanAlbada(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right);
            vec4 m_SL_Minbee(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right);
            vec4 m_GetSlopeLimitingR(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right);
            vec4 m_GetSlopeMeasure(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right);
            void m_ReconstructData(int f_iDirection);
            void m_GetReconstructedFluxes(int f_iDirection);
            void m_EvolveHalfTimeStep(int f_iDirection);
            void m_SetLimitingFunction();
            
            double m_GetEnergy(const vec4& f_vec4_P);
            vec4 m_GetPrimitives(const vec4& f_vec4_U);
            vec4 m_GetConserved(const vec4& f_vec4_P);
            vec4 m_vXFlux(const vec4& f_vec4_U);
            vec4 m_vYFlux(const vec4& f_vec4_U);
            
            void m_OutputToFile(std::ofstream& outputFile, const int& timestep, const double& time);
            
            void m_SetBoundaryConditions();
            void m_SetTimeStep();
            void m_Evolve();
};