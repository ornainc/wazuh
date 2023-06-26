#include "stageBuilderParse.hpp"

#include <algorithm>
#include <any>
#include <stdexcept>
#include <string>

#include "expression.hpp"
#include "registry.hpp"
#include <json/json.hpp>

namespace builder::internals::builders
{

Builder getStageBuilderParse(std::weak_ptr<Registry<Builder>> weakRegistry)
{
    return [weakRegistry](const std::any& definition, std::shared_ptr<defs::IDefinitions> definitions)
    {
        if (weakRegistry.expired())
        {
            throw std::runtime_error("Parse stage: Registry expired");
        }
        auto registry = weakRegistry.lock();

        // Assert value is as expected
        json::Json jsonDefinition;
        try
        {
            jsonDefinition = std::any_cast<json::Json>(definition);
        }
        catch (const std::exception& e)
        {
            throw std::runtime_error(fmt::format("Definition could not be converted to json: {}", e.what()));
        }
        if (!jsonDefinition.isObject())
        {
            throw std::runtime_error(
                fmt::format("Invalid json definition type: expected object but got {}", jsonDefinition.typeName()));
        }

        std::vector<base::Expression> parserExpressions;
        auto parseObj = jsonDefinition.getObject().value();

        std::transform(
            parseObj.begin(),
            parseObj.end(),
            std::back_inserter(parserExpressions),
            [registry, definitions](auto& tuple)
            {
                const auto& parserName = std::get<0>(tuple);
                const auto& parserValue = std::get<1>(tuple);
                base::Expression parserExpression;
                try
                {
                    parserExpression = registry->getBuilder("parser." + parserName)(parserValue, definitions);
                }
                catch (const std::exception& e)
                {
                    throw std::runtime_error(fmt::format("Error building parser \"{}\": {}", parserName, e.what()));
                }

                return parserExpression;
            });

        return base::Or::create("parse", parserExpressions);
    };
}

} // namespace builder::internals::builders
