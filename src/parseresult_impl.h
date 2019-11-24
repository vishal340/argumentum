// Copyright (c) 2018, 2019 Marko Mahnič
// License: MPL2. See LICENSE in the root of the project.

#pragma once

#include "exceptions.h"
#include "notifier.h"

namespace argparse {

CPPARGPARSE_INLINE ParseError::ParseError( std::string_view optionName, int code )
   : option( optionName )
   , errorCode( code )
{}

CPPARGPARSE_INLINE void ParseError::describeError( std::ostream& stream ) const
{
   switch ( errorCode ) {
      case UNKNOWN_OPTION:
         stream << "Error: Unknown option: '" << option << "'\n";
         break;
      case EXCLUSIVE_OPTION:
         stream << "Error: Only one option from an exclusive group can be set. '" << option
                << "'\n";
         break;
      case MISSING_OPTION:
         stream << "Error: A required option is missing: '" << option << "'\n";
         break;
      case MISSING_OPTION_GROUP:
         stream << "Error: A required option from a group is missing: '" << option << "'\n";
         break;
      case MISSING_ARGUMENT:
         stream << "Error: An argument is missing: '" << option << "'\n";
         break;
      case CONVERSION_ERROR:
         stream << "Error: The argument could not be converted: '" << option << "'\n";
         break;
      case INVALID_CHOICE:
         stream << "Error: The value is not in the list of valid values: '" << option << "'\n";
         break;
      case FLAG_PARAMETER:
         stream << "Error: Flag options do not accep parameters: '" << option << "'\n";
         break;
      case EXIT_REQUESTED:
         break;
      case ACTION_ERROR:
         stream << "Error: " << option << "\n";
         break;
      case INVALID_ARGV:
         stream << "Error: Parser input is invalid.\n";
         break;
      case INCLUDE_TOO_DEEP:
         stream << "Include depth exceeded: '" << option << "'\n";
         break;
   }
}

CPPARGPARSE_INLINE ParseResult::RequireCheck::RequireCheck( RequireCheck&& other )
{
   required = other.required;
   other.clear();
}

CPPARGPARSE_INLINE auto ParseResult::RequireCheck::operator=( RequireCheck&& other ) -> RequireCheck&
{
   required = other.required;
   other.clear();
   return *this;
}

CPPARGPARSE_INLINE ParseResult::RequireCheck::RequireCheck( bool require )
   : required( require )
{}

CPPARGPARSE_INLINE ParseResult::RequireCheck::~RequireCheck() noexcept( false )
{
   if ( required ) {
      if ( !std::current_exception() )
         throw UncheckedParseResult();   // lgtm [cpp/throw-in-destructor]
      else
         Notifier::warn( "Unchecked Parse Result." );
   }
}

CPPARGPARSE_INLINE void ParseResult::RequireCheck::activate()
{
   required = true;
}

CPPARGPARSE_INLINE void ParseResult::RequireCheck::clear()
{
   required = false;
}

CPPARGPARSE_INLINE ParseResult::~ParseResult() noexcept( false )
{}

CPPARGPARSE_INLINE bool ParseResult::has_exited() const
{
   return exitRequested;
}

CPPARGPARSE_INLINE bool ParseResult::help_was_shown() const
{
   return helpWasShown;
}

CPPARGPARSE_INLINE bool ParseResult::errors_were_shown() const
{
   return errorsWereShown;
}

CPPARGPARSE_INLINE ParseResult::operator bool() const
{
   mustCheck.clear();
   return errors.empty() && ignoredArguments.empty() && !exitRequested;
}

CPPARGPARSE_INLINE void ParseResult::clear()
{
   ignoredArguments.clear();
   errors.clear();
   mustCheck.clear();
   exitRequested = false;
}

CPPARGPARSE_INLINE void ParseResultBuilder::clear()
{
   mResult.clear();
}

CPPARGPARSE_INLINE bool ParseResultBuilder::wasExitRequested() const
{
   return mResult.exitRequested;
}

CPPARGPARSE_INLINE void ParseResultBuilder::addError( std::string_view optionName, int error )
{
   mResult.errors.emplace_back( optionName, error );
   mResult.mustCheck.activate();
}

CPPARGPARSE_INLINE void ParseResultBuilder::addIgnored( std::string_view arg )
{
   mResult.ignoredArguments.emplace_back( arg );
}

CPPARGPARSE_INLINE void ParseResultBuilder::addCommand( const std::shared_ptr<CommandOptions>& pCommand )
{
   mResult.commands.push_back( pCommand );
}

CPPARGPARSE_INLINE void ParseResultBuilder::requestExit()
{
   mResult.exitRequested = true;
   addError( {}, EXIT_REQUESTED );
}

CPPARGPARSE_INLINE void ParseResultBuilder::signalHelpShown()
{
   mResult.helpWasShown = true;
}

CPPARGPARSE_INLINE void ParseResultBuilder::signalErrorsShown()
{
   mResult.errorsWereShown = true;
}

CPPARGPARSE_INLINE ParseResult&& ParseResultBuilder::getResult()
{
   return std::move( mResult );
}

CPPARGPARSE_INLINE bool ParseResultBuilder::hasArgumentProblems() const
{
   return !mResult.errors.empty() || !mResult.ignoredArguments.empty();
}

CPPARGPARSE_INLINE void ParseResultBuilder::addResult( ParseResult&& result )
{
   mResult.exitRequested |= result.exitRequested;
   mResult.helpWasShown |= result.helpWasShown;
   mResult.errorsWereShown |= result.errorsWereShown;

   mResult.mustCheck.required |= result.mustCheck.required;
   result.mustCheck.required = false;

   for ( auto&& error : result.errors )
      mResult.errors.push_back( std::move( error ) );

   for ( auto&& arg : result.ignoredArguments )
      mResult.ignoredArguments.push_back( std::move( arg ) );
}

}   // namespace argparse
