//
// Created by lrfri on 18.05.2025.
//

#ifndef RAYLIBSTARTER_NEURALNETWORKAI_H
#define RAYLIBSTARTER_NEURALNETWORKAI_H

#include "GameTypes.h"
#include "Board.h"
#include "GameState.h"
#include <vector>
#include <random>

class NeuralNetwork {
public:
    NeuralNetwork(int inputSize, int hiddenSize, int outputSize);
    std::vector<float> forward(const std::vector<float>& input);
    void train(const std::vector<float>& input, const std::vector<float>& target, float learningRate = 0.01f);

private:
    struct Layer {
        std::vector<std::vector<float>> weights;
        std::vector<float> bias;
        std::vector<float> output;
    };

    Layer inputLayer;
    Layer hiddenLayer;
    Layer outputLayer;

    std::vector<float> sigmoid(const std::vector<float>& x);
    std::vector<float> sigmoidDerivative(const std::vector<float>& x);
};

class NeuralNetworkAI {
public:
    NeuralNetworkAI(AIDifficulty difficulty);

    void initialize();
    void update(float deltaTime, const GameState& playerState);
    void learnFromPlayerMove(float playerSolveTime, int wrongAttempts, int hintsUsed);
    float calculateMoveDelay(float playerAvgTime);
    std::vector<CellPosition> findSolution(const BoardPtr& board, int targetNumber);

    bool isReadyToMove() const;
    std::vector<CellPosition> getCurrentMove() const;

private:
    NeuralNetwork network;
    AIDifficulty difficulty;
    float moveTimer;
    float nextMoveDelay;
    std::vector<CellPosition> currentMove;
    std::vector<float> playerStats;
    bool readyToMove;

    float getTimeMultiplier() const;
    void makeDecision(const std::vector<float>& features);
};

#endif //RAYLIBSTARTER_NEURALNETWORKAI_H
