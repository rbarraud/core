/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 *
 * This file incorporates work covered by the following license notice:
 *
 *   Licensed to the Apache Software Foundation (ASF) under one or more
 *   contributor license agreements. See the NOTICE file distributed
 *   with this work for additional information regarding copyright
 *   ownership. The ASF licenses this file to you under the Apache
 *   License, Version 2.0 (the "License"); you may not use this file
 *   except in compliance with the License. You may obtain a copy of
 *   the License at http://www.apache.org/licenses/LICENSE-2.0 .
 */

#ifndef INCLUDED_FRAMEWORK_INC_UIELEMENT_PROGRESSBARWRAPPER_HXX
#define INCLUDED_FRAMEWORK_INC_UIELEMENT_PROGRESSBARWRAPPER_HXX

#include <helper/uielementwrapperbase.hxx>
#include <macros/generic.hxx>
#include <macros/xinterface.hxx>
#include <macros/xtypeprovider.hxx>

#include <com/sun/star/task/XStatusIndicator.hpp>
#include <com/sun/star/awt/XWindow.hpp>

#include <cppuhelper/weak.hxx>
#include <cppuhelper/weakref.hxx>

namespace framework{

class ProgressBarWrapper : public UIElementWrapperBase
{
    public:

        //  constructor / destructor

        ProgressBarWrapper();
        virtual ~ProgressBarWrapper();

        // public interfaces
        void setStatusBar( const ::com::sun::star::uno::Reference< ::com::sun::star::awt::XWindow >& rStatusBar, bool bOwnsInstance = false );
        ::com::sun::star::uno::Reference< ::com::sun::star::awt::XWindow > getStatusBar() const;

        // wrapped methods of ::com::sun::star::task::XStatusIndicator
        void start( const OUString& Text, ::sal_Int32 Range ) throw (css::uno::RuntimeException, std::exception);
        void end() throw (css::uno::RuntimeException, std::exception);
        void setText( const OUString& Text ) throw (css::uno::RuntimeException, std::exception);
        void setValue( ::sal_Int32 Value ) throw (css::uno::RuntimeException, std::exception);
        void reset() throw (::com::sun::star::uno::RuntimeException);

        // UNO interfaces
        // XComponent
        virtual void SAL_CALL dispose() throw (::com::sun::star::uno::RuntimeException, std::exception) SAL_OVERRIDE;

        // XInitialization
        virtual void SAL_CALL initialize( const ::com::sun::star::uno::Sequence< ::com::sun::star::uno::Any >& aArguments ) throw (::com::sun::star::uno::Exception, ::com::sun::star::uno::RuntimeException, std::exception) SAL_OVERRIDE;

        // XUpdatable
        virtual void SAL_CALL update() throw (::com::sun::star::uno::RuntimeException, std::exception) SAL_OVERRIDE;

        // XUIElement
        virtual ::com::sun::star::uno::Reference< ::com::sun::star::uno::XInterface > SAL_CALL getRealInterface() throw (::com::sun::star::uno::RuntimeException, std::exception) SAL_OVERRIDE;

    //  variables
    //  (should be private everyway!)

    private:
        ::com::sun::star::uno::Reference< ::com::sun::star::awt::XWindow >         m_xStatusBar;    // Reference to our status bar XWindow
        ::com::sun::star::uno::WeakReference< ::com::sun::star::uno::XInterface >  m_xProgressBarIfacWrapper;
        bool                                                                   m_bOwnsInstance; // Indicator that we are owner of the XWindow
        sal_Int32                                                                  m_nRange;
        sal_Int32                                                                  m_nValue;
        OUString                                                              m_aText;
};      //  class ProgressBarWrapper

}       //  namespace framework

#endif // INCLUDED_FRAMEWORK_INC_UIELEMENT_PROGRESSBARWRAPPER_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
