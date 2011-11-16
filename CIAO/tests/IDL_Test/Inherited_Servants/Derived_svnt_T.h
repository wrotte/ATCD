// -*- C++ -*-
// $Id$

/**
 *
 * Bla bla bla
 *
 */

// TAO_IDL - Generated from
// be/be_????.cpp:??

#ifndef CIAO_SESSION_DERIVED_SVNT_T_XXXXX_H_
#define CIAO_SESSION_DERIVED_SVNT_T_XXXXX_H_

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ciao/Servants/Facet_Servant_Base_T.h"

namespace CIAO_FACET_Inherited
{
  template <typename BASE, typename EXEC>
  class derived_interface_Servant_T
    : public CIAO::Facet_Servant_Base_T<BASE, EXEC, ::Components::SessionContext>
  {
  public:
    derived_interface_Servant_T (
      typename EXEC::_ptr_type exec,
      ::Components::CCMContext_ptr ctx);
    virtual ~derived_interface_Servant_T (void);

    virtual void do_derived (void);
  };
};

namespace CIAO_FACET_Inherited
{
  template <typename BASE, typename EXEC>
  class derived_2_interface_Servant_T
    : public derived_interface_Servant_T<BASE, EXEC>
  {
  public:
    derived_2_interface_Servant_T (
      typename EXEC::_ptr_type exec,
      ::Components::CCMContext_ptr ctx);
    virtual ~derived_2_interface_Servant_T (void);

    virtual void do_derived_2 (
      void);

  };
};

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Derived_svnt_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Derived_svnt_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"


#endif /* CIAO_SESSION_DERIVED_SVNT_T_XXXXX_H_ */
