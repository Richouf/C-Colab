#pragma once

#include <vector>
#include <cstdlib>



class Neuron;

typedef std::vector<Neuron> layer;

struct connection
{
    double weight;
    double deltaWeight;
};

class Net
{
public:
    Net(const std::vector<unsigned>& topology);
    Net(const std::vector<std::vector<std::vector<double>>>& pathMap);
    void feedForward(const std::vector<double>& inputVals);
    void backProp(const std::vector<double>& targetVals);
    void getResults(std::vector<double>& resultVals) const;
    double getRecentAverageError(void) const { return m_recentAverageError; }
    void getPathWeights(std::vector<std::vector<std::vector<double>>>& pathMap) const;

private:
    double m_error;
    double m_recentAverageError;
    static double m_recentAverageSmoothingFactor;
    std::vector<layer> m_layers; // m_layers[layerNum][neuronNum]
};

class Neuron
{
public:
    Neuron(unsigned numOutputs, unsigned myIndex);
    Neuron(std::vector<double>& pathMap, unsigned myIndex);
    void setOutputVal(double neuronVal) { m_outputVal = neuronVal; }
    double getOutputVal(void) const { return m_outputVal; }
    void neuronFeedForward(const layer& prevLayer);
    void calcOutputGradients(double targetVal);
    void calcHiddenGradients(const layer& nextLayer);
    void updateInputWeights(layer& prevLayer);
    void getNeuronPathWeights(std::vector<double>& neuronPathMap) const;

private:
    static double transferFunction(double x);
    static double transferFunctionDerivative(double x);
    static double randomWeight(void) { return rand() / double(RAND_MAX); }
    static double eta;
    static double alpha;
    double sumDOW(const layer& nextLayer) const;
    double m_outputVal;
    std::vector<connection> m_outputWeights;
    unsigned m_myIndex;
    double m_gradient;
};