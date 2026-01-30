#include "simulation.h"

void Simulation::SetLimitingFunction()
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

vec4 Simulation::m_GetSlopeLimitingR(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right)
    {
        const double eps = 1e-12;

        vec4 l_dNumerator = U_Center - U_Left;
        vec4 l_dDenominator = U_Right  - U_Center;

        vec4 l_vec3SlopeLimitingR;

        for (int k = 0; k < l_dNumerator.size(); ++k)
            {
                if (std::abs(l_dDenominator[k]) > eps)
                    {
                        l_vec3SlopeLimitingR[k] = l_dNumerator[k] / l_dDenominator[k];
                    }
                else
                    {
                        l_vec3SlopeLimitingR[k] = 0.0;
                    }
            }

        return l_vec3SlopeLimitingR;

    }

vec4 Simulation::m_SL_Superbee(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right)
    {
        vec4 l_dSlopeLimitingR = m_GetSlopeLimitingR(U_Left, U_Center, U_Right);
        vec4 m_dLeftSlopeLimit = (2.0 * l_dSlopeLimitingR) / (1.0 + l_dSlopeLimitingR);
        vec4 m_dRightSlopeLimit = 2.0 / (1.0 + l_dSlopeLimitingR);

        vec4 l_vResults = vec4();

        for (int i = 0; i < l_dSlopeLimitingR.size(); i++)
            {
            if (l_dSlopeLimitingR[i] <= 0.0)
                {
                    l_vResults[i] = 0.0;
                }
            else if (l_dSlopeLimitingR[i] > 0.0 && l_dSlopeLimitingR[i] <= 1.0/2.0)
                {
                    l_vResults[i] = 2.0 * l_dSlopeLimitingR[i];
                }
            else if (l_dSlopeLimitingR[i] > 1.0/2.0 and l_dSlopeLimitingR[i] <= 1.0)
                {
                    l_vResults[i] = 1.0;
                }
            else
                {
                    l_vResults[i] = std::min(l_dSlopeLimitingR[i], std::min(m_dRightSlopeLimit[i], 2.0));
                }
            }

        return l_vResults;
    }
    
vec4 Simulation::m_SL_VanLeer(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right)
    {
        vec4 l_dSlopeLimitingR = m_GetSlopeLimitingR(U_Left, U_Center, U_Right);
        vec4 m_dLeftSlopeLimit = (2.0 * l_dSlopeLimitingR) / (1.0 + l_dSlopeLimitingR);
        vec4 m_dRightSlopeLimit = 2.0 / (1.0 + l_dSlopeLimitingR);

        vec4 l_vResults = vec4();

        for (int i = 0; i < l_dSlopeLimitingR.size(); i++)
            {
                if (l_dSlopeLimitingR[i] <= 0.0)
                    {
                        l_vResults[i] = 0.0;
                    }
                else
                    {
                        l_vResults[i] = std::min(m_dLeftSlopeLimit[i], m_dRightSlopeLimit[i]);
                    }
            }

        return l_vResults;
    }

vec4 Simulation::m_SL_VanAlbada(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right)
    {
        vec4 l_dSlopeLimitingR = m_GetSlopeLimitingR(U_Left, U_Center, U_Right);
        vec4 m_dLeftSlopeLimit = (2.0 * l_dSlopeLimitingR) / (1.0 + l_dSlopeLimitingR);
        vec4 m_dRightSlopeLimit = 2.0 / (1.0 + l_dSlopeLimitingR);

        vec4 l_vResults = vec4();

        for (int i = 0; i < l_dSlopeLimitingR.size(); i++)
            {
                if (l_dSlopeLimitingR[i] <= 0.0)
                    {
                        l_vResults[i] = 0.0;
                    }
                else
                    {
                        double l_dFirstTerm = l_dSlopeLimitingR[i] * (1 + l_dSlopeLimitingR[i]) / (1 + l_dSlopeLimitingR[i] * l_dSlopeLimitingR[i]);
                        l_vResults[i] = std::min(l_dFirstTerm, m_dRightSlopeLimit[i]);
                    }
            }

        return l_vResults;
    }

vec4 Simulation::m_SL_Minbee(const vec4& U_Left, const vec4& U_Center, const vec4& U_Right)
    {
        vec4 l_dSlopeLimitingR = m_GetSlopeLimitingR(U_Left, U_Center, U_Right);
        vec4 m_dLeftSlopeLimit = (2.0 * l_dSlopeLimitingR) / (1.0 + l_dSlopeLimitingR);
        vec4 m_dRightSlopeLimit = 2.0 / (1.0 + l_dSlopeLimitingR);

        vec4 l_vResults = vec4();

        for (int i = 0; i < l_dSlopeLimitingR.size(); i++)
            {
                if (l_dSlopeLimitingR[i] <= 0.0)
                    {
                        l_vResults[i] = 0.0;
                    }
                else if (l_dSlopeLimitingR[i] > 0.0 && l_dSlopeLimitingR[i] <= 1.0)
                    {
                        l_vResults[i] = l_dSlopeLimitingR[i];
                    }
                else
                    {
                        l_vResults[i] = std::min(1.0, m_dRightSlopeLimit[i]);
                    }
            }

        return l_vResults;
    }