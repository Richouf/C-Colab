#include "Net.h"

Net::Net(const std::vector<unsigned>& topology)
{
    unsigned numLayers = topology.size();
    for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
    {
        m_layers.push_back(layer());
        unsigned numOutputs = layerNum == topology.size() - 1 ? 0 : topology[layerNum + 1];

        for (unsigned neuronNum = 0; neuronNum <= topology[layerNum]; ++neuronNum)
        {
            m_layers.back().push_back(Neuron(numOutputs, neuronNum));
        }
        m_layers.back().back().setOutputVal(1.0);
    }
}

Net::Net(const std::vector<std::vector<std::vector<double>>>& pathMap)
{
    unsigned numLayers = pathMap.size();
    for (unsigned layerNum = 0; layerNum < numLayers; ++layerNum)
    {
        m_layers.push_back(layer());
        unsigned numOutputs = layerNum == pathMap.size() - 1 ? 0 : pathMap[layerNum].size();

        for (unsigned neuronNum = 0; neuronNum <= pathMap[layerNum].size(); ++neuronNum)
        {
            std::vector<double> newPaths = pathMap[layerNum][neuronNum];
            m_layers.back().push_back(Neuron(newPaths, neuronNum));
        }
    }
}

void Net::feedForward(const std::vector<double>& inputVals)
{

    for (unsigned i = 0; i < inputVals.size(); ++i)
        m_layers[0][i].setOutputVal(inputVals[i]);

    for (unsigned layerNum = 1; layerNum < m_layers.size(); ++layerNum)
    {
        layer& prevLayer = m_layers[layerNum - 1];
        for (unsigned n = 0; n < m_layers[layerNum].size() - 1; n++)
        {
            m_layers[layerNum][n].neuronFeedForward(prevLayer);
        }
    }
}

void Net::backProp(const std::vector<double>& targetVals)
{
    layer& outputLayer = m_layers.back();
    m_error = 0.0;
    for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
    {
        double delta = targetVals[n] - outputLayer[n].getOutputVal();
        m_error += delta * delta;
    }
    m_error /= outputLayer.size() - 1;
    m_error = sqrt(m_error);

    m_recentAverageError = (m_recentAverageError * m_recentAverageSmoothingFactor + m_error) / (m_recentAverageSmoothingFactor + 1.0);

    for (unsigned n = 0; n < outputLayer.size() - 1; ++n)
        outputLayer[n].calcOutputGradients(targetVals[n]);

    for (unsigned layerNum = m_layers.size() - 2; layerNum > 0; --layerNum)
    {
        layer& hiddenLayer = m_layers[layerNum];
        layer& nextLayer = m_layers[layerNum + 1];

        for (unsigned n = 0; n < hiddenLayer.size(); ++n)
            hiddenLayer[n].calcHiddenGradients(nextLayer);
    }

    for (unsigned layerNum = m_layers.size() - 1; layerNum > 0; --layerNum)
    {
        layer& currentLayer = m_layers[layerNum];
        layer& prevLayer = m_layers[layerNum - 1];

        for (unsigned n = 0; n < currentLayer.size() - 1; n++)
            currentLayer[n].updateInputWeights(prevLayer);
    }
}

void Net::getResults(std::vector<double>& resultVals) const
{
    resultVals.clear();
    for (unsigned n = 0; n < m_layers.back().size() - 1; ++n)
        resultVals.push_back(m_layers.back()[n].getOutputVal());
}

void Net::getPathWeights(std::vector<std::vector<std::vector<double>>>& pathMap) const
{

    unsigned numLayers = m_layers.size();
    for (unsigned layerNum = 0; layerNum < numLayers - 1; ++layerNum)
    {
        std::vector<std::vector<double>> newLayer;
        pathMap.push_back(newLayer);
        layer currentLayer = m_layers[layerNum];
        unsigned numNeurons = currentLayer.size();
        for (unsigned neuronNum = 0; neuronNum < numNeurons; ++neuronNum)
        {
            std::vector<double> neuronPathMap;
            Neuron currentNeuron = currentLayer[neuronNum];
            currentNeuron.getNeuronPathWeights(neuronPathMap);
            pathMap.back().push_back(neuronPathMap);
        }
    }
}

Neuron::Neuron(unsigned numOutputs, unsigned myIndex)
{
    for (unsigned paths = 0; paths < numOutputs; ++paths)
    {
        m_outputWeights.push_back(connection());
        m_outputWeights.back().weight = randomWeight();
    }

    m_myIndex = myIndex;
}

Neuron::Neuron(std::vector<double>& neuronMap, unsigned myIndex)
{
    for (unsigned pathNum = 0; pathNum < neuronMap.size(); ++pathNum)
    {
        m_outputWeights.push_back(connection());
        m_outputWeights.back().weight = neuronMap[pathNum];
    }

    m_myIndex = myIndex;
}

void Neuron::neuronFeedForward(const layer& prevLayer)
{
    double sum = 0.0;
    for (unsigned n = 0; n < prevLayer.size(); ++n)
    {
        sum += prevLayer[n].getOutputVal() * prevLayer[n].m_outputWeights[m_myIndex].weight;
    }

    m_outputVal = Neuron::transferFunction(sum);
}

void Neuron::calcOutputGradients(double targetVal)
{
    double delta = targetVal - m_outputVal;
    m_gradient = delta * Neuron::transferFunctionDerivative(m_outputVal);
}

void Neuron::calcHiddenGradients(const layer& nextLayer)
{
    double dow = sumDOW(nextLayer);
    m_gradient = dow * Neuron::transferFunctionDerivative(m_outputVal);
}

void Neuron::updateInputWeights(layer& prevLayer)
{
    for (unsigned n = 0; n < prevLayer.size(); n++)
    {
        Neuron& neuron = prevLayer[n];
        double oldDeltaWeight = neuron.m_outputWeights[m_myIndex].deltaWeight;

        double newDeltaWeight = eta * neuron.getOutputVal() * m_gradient + alpha * oldDeltaWeight;
        //eta is overall net learning rate
        //alpha is momentum of learning rate

        neuron.m_outputWeights[m_myIndex].deltaWeight = newDeltaWeight;
        neuron.m_outputWeights[m_myIndex].weight += newDeltaWeight;
    }
}

void Neuron::getNeuronPathWeights(std::vector<double>& neuronPathWeight) const
{
    unsigned numPaths = m_outputWeights.size();
    for (unsigned pathNum = 0; pathNum < numPaths; ++pathNum)
    {
        neuronPathWeight.push_back(m_outputWeights[pathNum].weight);
    }
}

double Net::m_recentAverageSmoothingFactor = 100.0;

double Neuron::transferFunction(double x)
{
    return tanh(x);
}

double Neuron::transferFunctionDerivative(double x)
{
    return 1 - x * x;
}

double Neuron::eta = .15;

double Neuron::alpha = .5;

double Neuron::sumDOW(const layer& nextLayer) const
{
    double sum = 0.0;

    for (unsigned n = 0; n < nextLayer.size() - 1; ++n)
        sum += m_outputWeights[n].weight * nextLayer[n].m_gradient;

    return sum;
}