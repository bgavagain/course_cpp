#include "node.h"

#include <tuple>

using namespace std;

DateComparisonNode::DateComparisonNode(Comparison cmp, const Date& date) :  date_(date) {
	Node::cmp_ = cmp;
}

bool DateComparisonNode::Evaluate(const Date& date, const std::string& event) {
	auto t1 = tie(date.year_, date.month_, date.day_);
	auto t2 = tie(date_.year_, date_.month_, date_.day_);

	if (cmp_ == Comparison::Less) {
		return t1 < t2;
	}
	else if (cmp_ == Comparison::LessOrEqual) {
		return t1 <= t2;
	}
	else if (cmp_ == Comparison::Greater) {
		return t1 > t2;
	}
	else if (cmp_ == Comparison::GreaterOrEqual) {
		return t1 >= t2;
	}
	else if (cmp_ == Comparison::Equal) {
		return t1 == t2;
	}
	else if (cmp_ == Comparison::NotEqual) {
		return t1 != t2;
	}
	return false;
}

EventComparisonNode::EventComparisonNode(Comparison cmp, const std::string& event) : event_(event) {
	Node::cmp_ = cmp;
}

bool EventComparisonNode::Evaluate(const Date& date, const std::string& event) {
	if (cmp_ == Comparison::Less) {
		return event < event_;
	}
	else if (cmp_ == Comparison::LessOrEqual) {
		return event <= event_;
	}
	else if (cmp_ == Comparison::Greater) {
		return event > event_;
	}
	else if (cmp_ == Comparison::GreaterOrEqual) {
		return event >= event_;
	}
	else if (cmp_ == Comparison::Equal) {
		return event == event_;
	}
	else if (cmp_ == Comparison::NotEqual) {
		return event != event_;
	}
    return false;
}

LogicalOperationNode::LogicalOperationNode(LogicalOperation op, std::shared_ptr<Node> left,
    std::shared_ptr<Node> parsedExpr) : op_(op), left_(left), right_(parsedExpr) { }

bool LogicalOperationNode::Evaluate(const Date& date, const std::string& event) {
	if (op_ == LogicalOperation::And) {
		return (left_->Evaluate(date, event) && right_->Evaluate(date, event));
	}
	else if (op_ == LogicalOperation::Or) {
		return (left_->Evaluate(date, event) || right_->Evaluate(date, event));
	}
    return false;
}

