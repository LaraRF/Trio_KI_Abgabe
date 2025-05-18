//
// Created by lrfri on 18.05.2025.
//

#include "NeuralNetworkAI.h"
#include <cmath>
#include <algorithm>

// NeuralNetwork Implementation
NeuralNetwork::NeuralNetwork(int inputSize, int hiddenSize, int outputSize) {
    // Initialisiere Eingabeschicht
    inputLayer.output.resize(inputSize);

    // Initialisiere versteckte Schicht
    hiddenLayer.weights.resize(hiddenSize, std::vector<float>(inputSize));
    hiddenLayer.bias.resize(hiddenSize);
    hiddenLayer.output.resize(hiddenSize);

    // Initialisiere Ausgabeschicht
    outputLayer.weights.resize(outputSize, std::vector<float>(hiddenSize));
    outputLayer.bias.resize(outputSize);
    outputLayer.output.resize(outputSize);

    // Zufällige Initialisierung der Gewichte und Bias
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    for (auto& row : hiddenLayer.weights) {
        for (auto& weight : row) {
            weight = dis(gen) * 0.1f;
        }
    }

    for (auto& bias : hiddenLayer.bias) {
        bias = dis(gen) * 0.1f;
    }

    for (auto& row : outputLayer.weights) {
        for (auto& weight : row) {
            weight = dis(gen) * 0.1f;
        }
    }

    for (auto& bias : outputLayer.bias) {
        bias = dis(gen) * 0.1f;
    }
}

std::vector<float> NeuralNetwork::forward(const std::vector<float>& input) {
    // Kopiere Eingabe
    inputLayer.output = input;

    // Vorwärtsdurchlauf durch versteckte Schicht
    for (int i = 0; i < hiddenLayer.output.size(); i++) {
        float sum = 0.0f;
        for (int j = 0; j < inputLayer.output.size(); j++) {
            sum += inputLayer.output[j] * hiddenLayer.weights[i][j];
        }
        sum += hiddenLayer.bias[i];
        hiddenLayer.output[i] = 1.0f / (1.0f + std::exp(-sum)); // Sigmoid-Aktivierung
    }

    // Vorwärtsdurchlauf durch Ausgabeschicht
    for (int i = 0; i < outputLayer.output.size(); i++) {
        float sum = 0.0f;
        for (int j = 0; j < hiddenLayer.output.size(); j++) {
            sum += hiddenLayer.output[j] * outputLayer.weights[i][j];
        }
        sum += outputLayer.bias[i];
        outputLayer.output[i] = 1.0f / (1.0f + std::exp(-sum)); // Sigmoid-Aktivierung
    }

    return outputLayer.output;
}

void NeuralNetwork::train(const std::vector<float>& input, const std::vector<float>& target, float learningRate) {
    // Forward pass
    forward(input);

    // Berechne Output-Layer-Fehler
    std::vector<float> outputErrors(outputLayer.output.size());
    for (int i = 0; i < outputLayer.output.size(); i++) {
        float output = outputLayer.output[i];
        outputErrors[i] = (target[i] - output) * output * (1 - output);
    }

    // Berechne Hidden-Layer-Fehler
    std::vector<float> hiddenErrors(hiddenLayer.output.size(), 0.0f);
    for (int i = 0; i < hiddenLayer.output.size(); i++) {
        float sum = 0.0f;
        for (int j = 0; j < outputLayer.output.size(); j++) {
            sum += outputErrors[j] * outputLayer.weights[j][i];
        }
        hiddenErrors[i] = sum * hiddenLayer.output[i] * (1 - hiddenLayer.output[i]);
    }

    // Aktualisiere Output-Layer-Gewichte
    for (int i = 0; i < outputLayer.output.size(); i++) {
        for (int j = 0; j < hiddenLayer.output.size(); j++) {
            outputLayer.weights[i][j] += learningRate * outputErrors[i] * hiddenLayer.output[j];
        }
        outputLayer.bias[i] += learningRate * outputErrors[i];
    }

    // Aktualisiere Hidden-Layer-Gewichte
    for (int i = 0; i < hiddenLayer.output.size(); i++) {
        for (int j = 0; j < inputLayer.output.size(); j++) {
            hiddenLayer.weights[i][j] += learningRate * hiddenErrors[i] * inputLayer.output[j];
        }
        hiddenLayer.bias[i] += learningRate * hiddenErrors[i];
    }
}

std::vector<float> NeuralNetwork::sigmoid(const std::vector<float>& x) {
    std::vector<float> result(x.size());
    for (int i = 0; i < x.size(); i++) {
        result[i] = 1.0f / (1.0f + std::exp(-x[i]));
    }
    return result;
}

std::vector<float> NeuralNetwork::sigmoidDerivative(const std::vector<float>& x) {
    std::vector<float> result(x.size());
    for (int i = 0; i < x.size(); i++) {
        float sigmoid_x = 1.0f / (1.0f + std::exp(-x[i]));
        result[i] = sigmoid_x * (1 - sigmoid_x);
    }
    return result;
}

// NeuralNetworkAI Implementation
NeuralNetworkAI::NeuralNetworkAI(AIDifficulty difficulty)
        : network(3, 5, 1), difficulty(difficulty), moveTimer(0.0f), nextMoveDelay(2.0f), readyToMove(false) {
    playerStats.resize(3, 0.0f); // [avgTime, wrongAttempts, hintsUsed]
}

void NeuralNetworkAI::initialize() {
    moveTimer = 0.0f;
    nextMoveDelay = 2.0f;
    readyToMove = false;
    currentMove.clear();
    playerStats = {2.0f, 0.0f, 0.0f}; // Standardwerte
}

void NeuralNetworkAI::update(float deltaTime, const GameState& playerState) {
    if (readyToMove) return;

    moveTimer += deltaTime;

    if (moveTimer >= nextMoveDelay) {
        readyToMove = true;
    }
}

void NeuralNetworkAI::learnFromPlayerMove(float playerSolveTime, int wrongAttempts, int hintsUsed) {
    // Aktualisiere Spielerstatistiken
    playerStats[0] = playerSolveTime;
    playerStats[1] = static_cast<float>(wrongAttempts);
    playerStats[2] = static_cast<float>(hintsUsed);

    // Berechne das angestrebte Zeitverhalten basierend auf dem Schwierigkeitsgrad
    float targetTime = playerSolveTime * getTimeMultiplier();

    // Bereite die Ein- und Ausgabedaten für das Training vor
    std::vector<float> input = {
            playerSolveTime / 10.0f,        // Normalisiere auf 0-1 Bereich
            wrongAttempts / 5.0f,           // Normalisiere
            hintsUsed / 3.0f                // Normalisiere
    };

    std::vector<float> target = {targetTime / 10.0f}; // Normalisiere

    // Trainiere das Netzwerk
    network.train(input, target);

    // Berechne die nächste Bewegungsverzögerung
    makeDecision(input);
}

float NeuralNetworkAI::calculateMoveDelay(float playerAvgTime) {
    // Standardzeitverzögerung basierend auf Schwierigkeitsgrad
    float baseDelay = playerAvgTime * getTimeMultiplier();

    // Wende ein wenig Zufälligkeit an, um Vorhersehbarkeit zu vermeiden
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.8f, 1.2f);

    return baseDelay * dis(gen);
}

std::vector<CellPosition> NeuralNetworkAI::findSolution(const BoardPtr& board, int targetNumber) {
    // Verwende einen einfachen DFS-Algorithmus, um eine Lösung zu finden
    std::vector<CellPosition> solution;

    // Durchsuche alle möglichen Startzellen
    for (int row = 0; row < board->getSize(); row++) {
        for (int col = 0; col < board->getSize(); col++) {
            solution.push_back({row, col});

            // Für jede benachbarte Zelle
            std::vector<CellPosition> neighbors = board->getValidNextCells(solution);
            for (const auto& neighbor : neighbors) {
                solution.push_back(neighbor);

                // Für jede dritte mögliche Zelle
                std::vector<CellPosition> thirdCells = board->getValidNextCells(solution);
                for (const auto& third : thirdCells) {
                    solution.push_back(third);

                    // Überprüfe, ob diese Kombination eine Lösung ergibt
                    if (board->checkWinCondition(solution, targetNumber)) {
                        return solution; // Lösung gefunden
                    }

                    solution.pop_back(); // Entferne die dritte Zelle
                }

                solution.pop_back(); // Entferne die zweite Zelle
            }

            solution.pop_back(); // Entferne die erste Zelle
        }
    }

    return {}; // Keine Lösung gefunden
}

bool NeuralNetworkAI::isReadyToMove() const {
    return readyToMove;
}

std::vector<CellPosition> NeuralNetworkAI::getCurrentMove() const {
    return currentMove;
}

float NeuralNetworkAI::getTimeMultiplier() const {
    switch (difficulty) {
        case AIDifficulty::EASY:
            return 1.3f; // 30% langsamer als der Spieler
        case AIDifficulty::MEDIUM:
            return 1.0f; // Gleich schnell wie der Spieler
        case AIDifficulty::HARD:
            return 0.7f; // 30% schneller als der Spieler
        default:
            return 1.0f;
    }
}

void NeuralNetworkAI::makeDecision(const std::vector<float>& features) {
    // Berechne mithilfe des neuronalen Netzes
    std::vector<float> output = network.forward(features);

    // Denormalisiere die Ausgabe und setze die Verzögerung
    nextMoveDelay = output[0] * 10.0f; // Denormalisiere

    // Stelle sicher, dass die Verzögerung in einem vernünftigen Bereich liegt
    nextMoveDelay = std::max(0.5f, std::min(10.0f, nextMoveDelay));

    // Setze den Timer zurück
    moveTimer = 0.0f;
    readyToMove = false;
}