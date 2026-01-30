#include <iostream>

class vec4 {
    public:
        double m_dVars[4];

        vec4() : m_dVars{0.0, 0.0, 0.0, 0.0} {};
        vec4(double u_dVar0, double u_dVar1, double u_dVar2, double u_dVar3) : m_dVars{u_dVar0, u_dVar1, u_dVar2, u_dVar3} {};

        double m_dDensity() const {return m_dVars[0];};
        double m_dXMomentum() const {return m_dVars[1];};
        double m_dYMomentum() const {return m_dVars[2];};
        double m_dEnergy() const {return m_dVars[3];};        

        vec4 operator-() const {return vec4(-m_dVars[0], -m_dVars[1], -m_dVars[2], -m_dVars[3]);}
        double operator[](int u_iInt) const {return m_dVars[u_iInt];}
        double& operator[](int u_iInt) {return m_dVars[u_iInt];}
        
        int size()
            {
                return sizeof(m_dVars) / sizeof(m_dVars[0]);
            }

        vec4& operator+=(const vec4& u_vVec)
            {
                m_dVars[0] += u_vVec[0];
                m_dVars[1] += u_vVec[1];
                m_dVars[2] += u_vVec[2];
                m_dVars[3] += u_vVec[3];

                return *this;
            }
        
        vec4& operator*=(const double& t)
            {
                m_dVars[0] *= t;
                m_dVars[1] *= t;
                m_dVars[2] *= t;
                m_dVars[3] *= t;

                return *this;
            }
            
        vec4& operator/=(const double& t)
            {
                return *this *= 1/t;
            }
};


inline vec4 operator+(const vec4& u_vVector1, const vec4& u_vVector2)
    {
        return vec4(u_vVector1[0] + u_vVector2[0],
                    u_vVector1[1] + u_vVector2[1],
                    u_vVector1[2] + u_vVector2[2],
                    u_vVector1[3] + u_vVector2[3]);
    }


inline vec4 operator+(const double& u_dScalar, const vec4& u_vVector2)
    {
        return vec4(u_dScalar + u_vVector2[0],
                    u_dScalar + u_vVector2[1],
                    u_dScalar + u_vVector2[2],
                    u_dScalar + u_vVector2[3]);
    }


inline vec4 operator+(const vec4& u_vVector1, const double& u_dScalar)
    {
        return u_dScalar + u_vVector1;
    }


inline vec4 operator-(const vec4& u_vVector1, const vec4& u_vVector2)
    {
        return vec4(u_vVector1[0] - u_vVector2[0],
                    u_vVector1[1] - u_vVector2[1],
                    u_vVector1[2] - u_vVector2[2],
                    u_vVector1[3] - u_vVector2[3]);
    }


inline vec4 operator*(const vec4& u_vVector1, const vec4& u_vVector2)
    {
        return vec4(u_vVector1[0]*u_vVector2[0],
                    u_vVector1[1]*u_vVector2[1],
                    u_vVector1[2]*u_vVector2[2],
                    u_vVector1[3]*u_vVector2[3]);
    }


inline vec4 operator*(const vec4& u_vVector, const double& u_dScalar)
    {
        return vec4(u_dScalar*u_vVector[0], u_dScalar*u_vVector[1], u_dScalar*u_vVector[2], u_dScalar * u_vVector[3]);
    }


inline vec4 operator*(const double& u_dScalar, const vec4& u_vVector)
    { 
        return u_vVector * u_dScalar;
    }


inline vec4 operator/(const vec4& u_vVector1, const vec4& u_vVector2)
    {
        return vec4(u_vVector1[0] / u_vVector2[0],
                    u_vVector1[1] / u_vVector2[1],
                    u_vVector1[2] / u_vVector2[2],
                    u_vVector1[3] / u_vVector2[3]);
    }


inline vec4 operator/(const double& u_dScalar, const vec4& u_vVector)
    {
        return vec4(
                u_dScalar / u_vVector[0], 
                u_dScalar / u_vVector[1], 
                u_dScalar / u_vVector[2],
                u_dScalar / u_vVector[3]);

    }


inline std::ostream& operator<<(std::ostream& os, const vec4& v) {
    os << v[0] << " " << v[1] << " " << v[2] << " " << v[3];
    return os;
}