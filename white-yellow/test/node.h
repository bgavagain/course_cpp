#pragma once

#include "date.h"

#include <memory>

enum class LogicalOperation {
    And,
    Or,
};

enum class Comparison {
    Less,
    LessOrEqual,
    Greater,
    GreaterOrEqual,
    Equal,
    NotEqual,
};

class Node {
public:
    int Evaluate(const Date& date, const std::string& event) { return 0; } //TODO:
};

class EmptyNode : public Node {

};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(Comparison cmp, const Date& date);

};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(Comparison comp, const std::string& event);

};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(LogicalOperation op, std::shared_ptr<Node> left,
        std::shared_ptr<Node> parsedExpr);
};
