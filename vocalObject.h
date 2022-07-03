
#pragma once

#include <QObject>

class VocalObject : public QObject
{
    Q_OBJECT

public:
    VocalObject(QString objName = "",
                QString objUnits = "",
                QString objDescription = "",
                int     digitCount = 4) :
        m_Max(100),
        m_Min(0),
        m_DialMajorStepSize(10),
        m_DialMinorStepSize(1),
        m_Name(objName),
        m_Units(objUnits),
        m_Description(objDescription),
        m_Digits(digitCount) {}
    double getValueDouble();
    float getValueFloat();
    int getValueInt();
    QString getName();
    QString getUnits();
    QString getDescription();
    void setName(QString newName);
    void setUnits(QString newUnit);
    void setDescription(QString newDescription);

    double getMaxDouble();
    float getMaxFloat();
    int getMaxInt();

    double getMinDouble();
    float getMinFloat();
    int getMinInt();

    double getDialMajorStepSize();
    double getDialMinorStepSize();

    int getDigitCount();
    void setDigitCount(int digitCount);

public slots:
    void setValueDouble(double val);
    void setValueFloat(float val);
    void setValueInt(int val);

    void setMaxDouble(double val);
    void setMaxFloat(float val);
    void setMaxInt(int val);

    void setMinDouble(double val);
    void setMinFloat(float val);
    void setMinInt(int val);

    void setDialMajorStepSize(double val);
    void setDialMinorStepSize(double val);

signals:
    void valueChangedDouble(double newValue);
    void valueChangedFloat(float newValue);
    void valueChangedInt(int newValue);

private:
    double  m_Value;
    double  m_Max;
    double  m_Min;
    double  m_DialMajorStepSize;
    double  m_DialMinorStepSize;
    QString m_Name;
    QString m_Units;
    QString m_Description;
    int     m_Digits;
};
