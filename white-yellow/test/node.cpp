#include "node.h"


DateComparisonNode::DateComparisonNode(Comparison cmp, const Date& date) {}

EventComparisonNode::EventComparisonNode(Comparison comp, const std::string& event) {}

LogicalOperationNode::LogicalOperationNode(LogicalOperation op, std::shared_ptr<Node> left,
    std::shared_ptr<Node> parsedExpr) {}