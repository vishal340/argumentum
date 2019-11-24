// Copyright (c) 2018, 2019 Marko Mahnič
// License: MPL2. See LICENSE in the root of the project.

#pragma once

#include "parserconfig.h"
#include "parserdefinition.h"

#include <set>
#include <string>
#include <string_view>
#include <vector>

namespace argparse {

class Option;
class Command;
class ParseResultBuilder;
class ArgumentStream;

class Parser
{
   ParserDefinition& mParserDef;
   ParseResultBuilder& mResult;

   bool mIgnoreOptions = false;
   int mPosition = 0;
   // The active option will receive additional argument(s)
   Option* mpActiveOption = nullptr;

public:
   Parser( ParserDefinition& argParser, ParseResultBuilder& result );
   void parse( ArgumentStream& argStream );

private:
   void startOption( std::string_view name );
   bool haveActiveOption() const;
   void closeOption();
   void addFreeArgument( std::string_view arg );
   void addError( std::string_view optionName, int errorCode );
   void setValue( Option& option, std::string_view value );

   void parse( ArgumentStream& argStream, unsigned depth );
   void parseCommandArguments(
         Command& command, ArgumentStream& argStream, ParseResultBuilder& result );
   void parseSubstream( std::string_view streamName, unsigned depth );
};

}   // namespace argparse