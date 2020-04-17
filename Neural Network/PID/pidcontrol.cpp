#include "pidcontrol.h"

pidcontrol::pidcontrol(const double& input, const double& setPoint,  double& output)
{
	m_feedback = &input;
	m_setPoint = &setPoint;
	m_output = &output;
	m_feedbackVal.min = 0;
	m_feedbackVal.max = 1024;
	m_feedbackReal.min = 0;
	m_feedbackReal.max = 100;
	m_outputReal.min = 0;
	m_outputReal.max = 100;
	m_outputVal.min = 0;
	m_outputVal.max = 2048;
	m_feedbackUnit[0] = 'mps';
	m_outputUnit[0] = 'N';
}

void pidcontrol::setGain(double Pgain, double Igain, double Dgain)
{
	m_Pg = Pgain; m_Ig = Igain; m_Dg = Dgain;
}

void pidcontrol::runPID()
{
	calcError();
	calcP();
	calcI();
	calcD();
	double PVal = m_POn ? m_PVal[0] : 0;
	double IVal = m_IOn ? m_IVal[0] : 0;
	double DVal = m_DOn ? m_DVal[0] : 0;
	double output = PVal + IVal + DVal;
	output = output <= m_outputReal.min ? m_outputReal.min : output >= m_outputReal.max ? m_outputReal.max : output;
	*m_output = output * ( m_outputVal.max - m_outputVal.min ) / ( m_outputReal.max - m_outputReal.min ) - m_outputVal.min;
}

void pidcontrol::calcError()
{
	for (int i = 9; i > 0; --i)
	{
		m_error[i] = m_error[i - 1];
		m_feedbackVals[i] = m_feedbackVals[i - 1];
	}
	double feedback = *m_feedback;
	feedback = feedback * ( m_feedbackReal.max - m_feedbackReal.min ) / ( m_feedbackVal.max - m_feedbackVal.min );
	m_error[0] = *m_setPoint - feedback;
	m_feedbackVals[0] = feedback;
}

void pidcontrol::calcP()
{
	for (int i = 9; i > 0; --i)
		m_PVal[i] = m_PVal[i - 1];
	m_PVal[0] = m_Pg * m_error[0] * m_dt;
}

void pidcontrol::calcI()
{
	for (int i = 9; i > 0; --i)
		m_IVal[i] = m_IVal[i - 1];
	m_IVal[0] =  m_useGain ? m_IVal[1] + m_Ig * m_error[0] * m_dt : m_IVal[1] + m_Pg * m_error[0] * m_dt / m_Ig;

}

void pidcontrol::calcD()
{
	if (m_DOnError)
	{
		for (int i = 9; i > 0; --i)
			m_DVal[i] = m_DVal[i - 1];
		m_DVal[0] = m_useGain ? m_Dg * (-m_error[0] + m_error[1] * 2 - m_error[2]) : m_dt * m_Pg * m_Dg * (-m_error[0] + m_error[1] * 2 - m_error[2]);
	}
	else
	{
		for (int i = 9; i > 0; --i)
			m_DVal[i] = m_DVal[i - 1];
		m_DVal[0] = m_useGain ? m_Dg * (-m_feedbackVals[0] + m_feedbackVals[1] * 2 - m_feedbackVals[2]) : m_dt * m_Pg * m_Dg * (-m_feedbackVals[0] + m_feedbackVals[1] * 2 - m_feedbackVals[2]);
	}
}

void pidcontrol::setRange(int rangeName, double min, double max)
{
	switch (rangeName)
	{
	case 1:
		m_feedbackVal.min = min;
		m_feedbackVal.max = max;
//		char m_feedbackUnit[20];
		break;
	case 2:
		m_outputVal.min = min;
		m_outputVal.max = max;
		break;
	case 3:
		m_feedbackReal.min = min;
		m_feedbackReal.max = max;
		break;
	case 4:
		m_outputReal.min = min;
		m_outputReal.max = max;
		break;
	default:
		;
	}
}

void pidcontrol::changeSetting(int setting, bool turnOn)
{
	switch (setting)
	{
	case 1:
		m_POn = turnOn;
		break;
	case 2:
		m_IOn = turnOn;
		break;
	case 3:
		m_DOn = turnOn;
		break;
	case 4:
		m_DOnError = turnOn;
		break;
	case 5:
		m_useGain = turnOn;
	default:
		;
	}
}

bool pidcontrol::getHist( double& historyArray, const int numVals)
{
	if (numVals >= valsKept)
		return false;
	else
	{
		double outputArray[4][valsKept];
		for (int i = 0; i < numVals; ++i)
		{
			outputArray[0][i] = m_PVal[i] + m_IVal[i] + m_DVal[i];
			outputArray[1][i] = m_PVal[i];
			outputArray[2][i] = m_IVal[i];
			outputArray[3][i] = m_DVal[i];
		}
		historyArray = outputArray[0][0];
		return true;
	}
}

bool pidcontrol::getHist( double& historyArray)
{
	double outputArray[4][valsKept];
	for (int i = 0; i < valsKept; ++i)
	{
		outputArray[0][i] = m_PVal[i] + m_IVal[i] + m_DVal[i];
		outputArray[1][i] = m_PVal[i];
		outputArray[2][i] = m_IVal[i];
		outputArray[3][i] = m_DVal[i];
	}
	historyArray = outputArray[0][0];
	return true;
}