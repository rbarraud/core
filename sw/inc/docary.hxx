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
#ifndef INCLUDED_SW_INC_DOCARY_HXX
#define INCLUDED_SW_INC_DOCARY_HXX

#include <com/sun/star/i18n/ForbiddenCharacters.hpp>
#include <vector>
#include <set>
#include <algorithm>
#include <o3tl/sorted_vector.hxx>

#include "charfmt.hxx"
#include "fldbas.hxx"
#include "fmtcol.hxx"
#include "frmfmt.hxx"
#include "numrule.hxx"
#include "section.hxx"
#include "tox.hxx"
#include "unocrsr.hxx"

class SwRangeRedline;
class SwExtraRedline;

namespace com { namespace sun { namespace star { namespace i18n {
    struct ForbiddenCharacters;    ///< comes from the I18N UNO interface
}}}}

#include <swtypes.hxx>
#include <ndarr.hxx>

/** provides some methods for generic operations on lists that contain
SwFmt* subclasses. */
class SwFmtsBase
{
public:
    virtual size_t GetFmtCount() const = 0;
    virtual SwFmt* GetFmt(size_t) const = 0;
    virtual ~SwFmtsBase() {}
};

template<typename Value>
class SwFmtsBaseModify : public std::vector<Value>, public SwFmtsBase
{
public:
    typedef typename std::vector<Value>::const_iterator const_iterator;

private:
    const bool mCleanup;

public:
    SwFmtsBaseModify(bool cleanup = true) : mCleanup(cleanup) {}

    using std::vector<Value>::begin;
    using std::vector<Value>::end;

    // free any remaining child objects based on mCleanup
    virtual ~SwFmtsBaseModify()
    {
        if (mCleanup)
            for(const_iterator it = begin(); it != end(); ++it)
                delete *it;
    }

    sal_uInt16 GetPos(Value const& p) const
    {
        const_iterator const it = std::find(begin(), end(), p);
        return it == end() ? USHRT_MAX : it - begin();
    }
    bool Contains(Value const& p) const
        { return std::find(begin(), end(), p) != end(); }
    virtual size_t GetFmtCount() const SAL_OVERRIDE
        { return std::vector<Value>::size(); }
    virtual SwFmt* GetFmt(size_t idx) const SAL_OVERRIDE
        { return (SwFmt*) std::vector<Value>::operator[](idx); }
    void dumpAsXml(xmlTextWriterPtr) {};
};

class SwGrfFmtColls : public SwFmtsBaseModify<SwGrfFmtColl*>
{
public:
    SwGrfFmtColls() : SwFmtsBaseModify( false ) {}
    virtual ~SwGrfFmtColls() {}
};

/// Specific frame formats (frames, DrawObjects).
class SW_DLLPUBLIC SwFrmFmts : public SwFmtsBaseModify<SwFrmFmt*>
{
public:
    virtual ~SwFrmFmts() {}
    void dumpAsXml(xmlTextWriterPtr w, const char* pName);
};

class SwCharFmts : public SwFmtsBaseModify<SwCharFmt*>
{
public:
    virtual ~SwCharFmts() {}
    void dumpAsXml(xmlTextWriterPtr w);
};

class SwTxtFmtColls : public SwFmtsBaseModify<SwTxtFmtColl*>
{
public:
    SwTxtFmtColls() : SwFmtsBaseModify( false ) {}
    virtual ~SwTxtFmtColls() {}
    void dumpAsXml(xmlTextWriterPtr w);
};

/// Array of Undo-history.
class SW_DLLPUBLIC SwSectionFmts : public SwFmtsBaseModify<SwSectionFmt*>
{
public:
    virtual ~SwSectionFmts() {}
    void dumpAsXml(xmlTextWriterPtr w);
};

class SwFldTypes : public SwFmtsBaseModify<SwFieldType*>
{
public:
    virtual ~SwFldTypes() {}
    void dumpAsXml(xmlTextWriterPtr w);
};

class SwTOXTypes : public SwFmtsBaseModify<SwTOXType*>
{
public:
    virtual ~SwTOXTypes() {}
};

class SW_DLLPUBLIC SwNumRuleTbl : public SwFmtsBaseModify<SwNumRule*> {
public:
    virtual ~SwNumRuleTbl() {}
    void dumpAsXml(xmlTextWriterPtr w);
};

struct CompareSwRedlineTbl
{
    bool operator()(SwRangeRedline* const &lhs, SwRangeRedline* const &rhs) const;
};

class _SwRedlineTbl
    : public o3tl::sorted_vector<SwRangeRedline*, CompareSwRedlineTbl,
                o3tl::find_partialorder_ptrequals>
{
public:
    ~_SwRedlineTbl();
};

class SwRedlineTbl : private _SwRedlineTbl
{
public:
    bool Contains(const SwRangeRedline* p) const { return find(const_cast<SwRangeRedline* const>(p)) != end(); }
    sal_uInt16 GetPos(const SwRangeRedline* p) const;

    bool Insert( SwRangeRedline* p, bool bIns = true );
    bool Insert( SwRangeRedline* p, sal_uInt16& rInsPos, bool bIns = true );
    bool InsertWithValidRanges( SwRangeRedline* p, sal_uInt16* pInsPos = 0 );

    void Remove( sal_uInt16 nPos );
    bool Remove( const SwRangeRedline* p );
    void DeleteAndDestroy( sal_uInt16 nPos, sal_uInt16 nLen = 1 );
    void DeleteAndDestroyAll();

    void dumpAsXml(xmlTextWriterPtr w);

    /** Search next or previous Redline with the same Seq. No.
       Search can be restricted via Lookahaed.
       Using 0 or USHRT_MAX makes search the whole array. */
    sal_uInt16 FindNextOfSeqNo( sal_uInt16 nSttPos, sal_uInt16 nLookahead = 20 ) const;
    sal_uInt16 FindPrevOfSeqNo( sal_uInt16 nSttPos, sal_uInt16 nLookahead = 20 ) const;
    sal_uInt16 FindNextSeqNo( sal_uInt16 nSeqNo, sal_uInt16 nSttPos,
                            sal_uInt16 nLookahead = 20 ) const;
    sal_uInt16 FindPrevSeqNo( sal_uInt16 nSeqNo, sal_uInt16 nSttPos,
                            sal_uInt16 nLookahead = 20 ) const;

    /**
     Find the redline at the given position.

     @param tableIndex position in SwRedlineTbl to start searching at, will be updated with the index of the returned
                       redline (or the next redline after the given position if not found)
     @param next true: redline starts at position and ends after, false: redline starts before position and ends at or after
    */
    const SwRangeRedline* FindAtPosition( const SwPosition& startPosition, sal_uInt16& tableIndex, bool next = true ) const;

    using _SwRedlineTbl::size;
    using _SwRedlineTbl::operator[];
    using _SwRedlineTbl::empty;
};

/// Table that holds 'extra' redlines, such as 'table row insert\delete', 'paragraph moves' etc...
class SwExtraRedlineTbl
{
private:
    std::vector<SwExtraRedline*>    m_aExtraRedlines;

public:
    ~SwExtraRedlineTbl();

    bool Insert( SwExtraRedline* p );

    void DeleteAndDestroy( sal_uInt16 nPos, sal_uInt16 nLen = 1 );
    void DeleteAndDestroyAll();

    void dumpAsXml(xmlTextWriterPtr w);

    sal_uInt16 GetSize() const                              {     return m_aExtraRedlines.size();                }
    SwExtraRedline* GetRedline( sal_uInt16 uIndex ) const   {     return m_aExtraRedlines.operator[]( uIndex );  }
    bool IsEmpty() const                              {     return m_aExtraRedlines.empty();               }

    bool DeleteAllTableRedlines( SwDoc* pDoc, const SwTable& rTable, bool bSaveInUndo, sal_uInt16 nRedlineTypeToDelete );
    bool DeleteTableRowRedline ( SwDoc* pDoc, const SwTableLine& rTableLine, bool bSaveInUndo, sal_uInt16 nRedlineTypeToDelete );
    bool DeleteTableCellRedline( SwDoc* pDoc, const SwTableBox& rTableBox, bool bSaveInUndo, sal_uInt16 nRedlineTypeToDelete );
};

class SwUnoCrsrTbl : public std::set<SwUnoCrsr*> {
public:
    /// the destructor will free all objects still in the set
    ~SwUnoCrsrTbl();
};

class SwOLENodes : public std::vector<SwOLENode*> {};

#endif // INCLUDED_SW_INC_DOCARY_HXX

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
