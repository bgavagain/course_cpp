#pragma once

#include "date.h"

#include <string>
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
    virtual bool Evaluate(const Date& date, const std::string& event) = 0;
protected:
	Comparison cmp_;
};

class EmptyNode : public Node {
    bool Evaluate(const Date& date, const std::string& event) override { return true; }
};

class DateComparisonNode : public Node {
public:
    DateComparisonNode(Comparison cmp, const Date& date);

    bool Evaluate(const Date& date, const std::string& event) override;
private:
	Date date_;
};

class EventComparisonNode : public Node {
public:
    EventComparisonNode(Comparison cmp, const std::string& event);
	bool Evaluate(const Date& date, const std::string& event) override;
private:
	std::string event_;
};

class LogicalOperationNode : public Node {
public:
    LogicalOperationNode(LogicalOperation op, std::shared_ptr<Node> left,
        std::shared_ptr<Node> parsedExpr);

    bool Evaluate(const Date& date, const std::string& event) override;
private:
	LogicalOperation op_;
	std::shared_ptr<Node> left_;
	std::shared_ptr<Node> right_;
};
