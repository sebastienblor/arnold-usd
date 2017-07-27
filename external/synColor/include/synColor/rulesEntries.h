//
// ==================================================================
// Copyright 2016 Autodesk, Inc.  All rights reserved.
//
// This computer source code  and related  instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are  protected  under applicable  copyright and trade secret
// law. They may not  be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
// ==================================================================
//
//! \file synColor/rulesEntries.h
//!
//! \brief Rules entries container class.

#ifndef _SYNCOLOR_RULES_ENTRIES_H
#define _SYNCOLOR_RULES_ENTRIES_H

#include <synColor/synColorRules.h>
#include <synColor/synStatus.h>

#include <vector>

namespace SYNCOLOR
{
  namespace Impl
  {
    //------------------------------------------------------------------------------------------------
    //! Container class that manages a list of rules. This class does not serialize nor does it
    //! populate from disk. It is up to the user to do this.
    //
    class RulesEntries
    {
    public:
         //! \brief Constructor
         RulesEntries();

         //! Destructor
         ~RulesEntries();

         //! \brief Copy Constructor
         RulesEntries(const RulesEntries& rhs);
       
         //! \brief Assignment operator
         RulesEntries& operator=(const RulesEntries& rhs);

         //! \brief Return the rule at a specified index. This is a direct access to the internal rule
         //!        list. There is not error handling.
         //! param i The index
         //! \return The rule pointer
         const SYNCOLOR::RulePtr& operator[](unsigned i) const;
     
         //! \brief Clear the list
         void clear();

         //! \brief Get the number of rules in the managed list
         //! \return Number of rules
         unsigned int size() const;

         //! \brief Get a specific rule from the list by index
         //! param position position of the rule in the list
         //! param rule Returned rule pointer
         //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
         //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
         SYNCOLOR::SynStatus getRule(unsigned int position, SYNCOLOR::RulePtr& rule) const;

         //! \brief Get a specific rule from the list by name
         //! param name name of the rule to return
         //! param rule Returned rule pointer
         //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
         //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
         SYNCOLOR::SynStatus getRule(const char* name, SYNCOLOR::RulePtr& rule) const;

         //! \brief Move a rule in the list
         //! param name name of the rule to move
         //! param offset How to move the rule
         //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
         //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
         SYNCOLOR::SynStatus moveRule(const char* name, int offset);

         //! \brief Add a rule at the end of the list
         //! param rule The rule to add
         //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
         //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
         SYNCOLOR::SynStatus addRule(const SYNCOLOR::RulePtr& rule);
       
         //! \brief Insert a rule in the list
         //! param position Where to add the rule
         //! param rule The rule to add
         //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
         //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
         SYNCOLOR::SynStatus insertRule(unsigned int position, const SYNCOLOR::RulePtr& rule);

         //! \brief Remove a rule from the list
         //! param name Name of the rule to remove
         //! \return returns a SYNCOLOR::SynStatus.  Check the error code to
         //!         determine if an error occurred. See SYNCOLOR::SynStatus above.
         SYNCOLOR::SynStatus removeRule(const char* name);

    private:
         typedef std::vector<SYNCOLOR::RulePtr> Entries;
         Entries _entries;
    };
  }
}

#endif
