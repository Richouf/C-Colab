#pragma once

struct range
{
	double min;
	double max;
};

enum class rangeNames { FeedbackVals = 1, OutputVals, FeedbackUnits, OutputUnits };

enum class settingNames { POn = 1, IOn, DOn, DOnError, GainBasis };

static const int valsKept = 10;

class pidcontrol
{
public:
	pidcontrol(const double& feedback, const double& setPoint, double& output);
	void setGain(double Pgain, double Igain, double Dgain);
	void setRange(int rangeName, double min, double max);
	void changeSetting(int setting, bool turnOn);
	void runPID();
	double getP() { return m_PVal[0]; };
	double getI() { return m_IVal[0]; };
	double getD() { return m_DVal[0]; };
	double getError() { return m_error[0]; };
	double getfbReal() { return *m_feedback * (m_feedbackReal.max - m_feedbackReal.min) / (m_feedbackVal.max - m_feedbackVal.min); };
	bool getHist( double& historyArray, int numVals);
	bool getHist( double& historyArray);
private:
	void calcP();
	void calcI();
	void calcD();
	void calcError();
	bool m_useGain = true;
	double m_Pg = 0;
	bool m_POn = true;
	double m_PVal[valsKept] = { 0 };
	double m_Ig = 0;
	bool m_IOn = true;
	double m_IVal[valsKept] = { 0 };
	double m_Dg = 0;
	bool m_DOn = true;
	double m_DVal[valsKept] = { 0 };
	bool m_DOnError = false;
	double m_error[valsKept] = { 0 };
	double m_feedbackVals[valsKept] = { 0 };
	const double* m_feedback;
	double* m_output;
	const double* m_setPoint;
	double m_dt = .1;
	range m_feedbackVal;
	range m_feedbackReal;
	char m_feedbackUnit[20];
	range m_outputVal;
	range m_outputReal;
	char m_outputUnit[20];
};

