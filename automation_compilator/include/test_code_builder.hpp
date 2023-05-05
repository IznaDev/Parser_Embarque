#pragma once

#include "code_builder.hpp"
#include <set>

class TestCodeBuilder : public ICodeBuilder
{

    public:
        void build(const json& json) override;
};