#include "vocalObject.h"

void VocalObject::setValueDouble(double val)
{
    if (val != this->m_Value)
    {
        this->m_Value = val;

        // notify listeners
        emit valueChangedDouble(val);
        emit valueChangedFloat(static_cast<float>(val));
        emit valueChangedInt(static_cast<int>(val));
    }
}

void VocalObject::setValueFloat(float val)
{
    double newVal = static_cast<double>(val);

    if (newVal != m_Value)
    {
        this->m_Value = newVal;

        // notify listeners
        emit valueChangedDouble(newVal);
        emit valueChangedFloat(val);
        emit valueChangedInt(static_cast<int>(val));
    }
}

void VocalObject::setValueInt(int val)
{
    double newVal = static_cast<double>(val);

    if (newVal != m_Value)
    {
        this->m_Value = newVal;

        // notify listeners
        emit valueChangedDouble(newVal);
        emit valueChangedFloat(static_cast<int>(val));
        emit valueChangedInt(val);
    }
}

///////////////////////////////////

double VocalObject::getValueDouble()
{
    return m_Value;
}

float VocalObject::getValueFloat()
{
    return static_cast<float>(m_Value);
}

int VocalObject::getValueInt()
{
    return static_cast<int>(m_Value);
}

///////////////////////////////////

QString VocalObject::getName()
{
    return m_Name;
}

QString VocalObject::getUnits()
{
    return m_Units;
}

QString VocalObject::getDescription()
{
    return m_Description;
}

///////////////////////////////////

void VocalObject::setName(QString newName)
{
    m_Name = newName;
}

void VocalObject::setUnits(QString newUnit)
{
    m_Units = newUnit;
}

void VocalObject::setDescription(QString newDescription)
{
    m_Description = newDescription;
}

///////////////////////////////////

void VocalObject::setMaxDouble(double val)
{
    m_Max = val;
}

void VocalObject::setMaxFloat(float val)
{
    m_Max = (double)val;
}

void VocalObject::setMaxInt(int val)
{
    m_Max = (double)val;
}

void VocalObject::setMinDouble(double val)
{
    m_Min = val;
}

void VocalObject::setMinFloat(float val)
{
    m_Min = (double)val;
}

void VocalObject::setMinInt(int val)
{
    m_Min = (double)val;
}

double VocalObject::getMaxDouble()
{
    return m_Max;
}

float VocalObject::getMaxFloat()
{
    return (float)m_Max;
}

int VocalObject::getMaxInt()
{
    return (int)m_Max;
}

double VocalObject::getMinDouble()
{
    return m_Min;
}

float VocalObject::getMinFloat()
{
    return (float)m_Min;
}

int VocalObject::getMinInt()
{
    return (int)m_Min;
}

double VocalObject::getDialMajorStepSize()
{
    return m_DialMajorStepSize;
}

double VocalObject::getDialMinorStepSize()
{
    return m_DialMinorStepSize;
}

void VocalObject::setDialMajorStepSize(double val)
{
    m_DialMajorStepSize = val;
}

void VocalObject::setDialMinorStepSize(double val)
{
    m_DialMinorStepSize = val;
}

int VocalObject::getDigitCount()
{
    return m_Digits;
}

void VocalObject::setDigitCount(int digitCount)
{
    m_Digits = digitCount;
}
