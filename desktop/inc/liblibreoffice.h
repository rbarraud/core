/* -*- Mode: C; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4 -*- */
/*
 * This file is part of the LibreOffice project.
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#ifndef INCLUDED_DESKTOP_INC_LIBLIBREOFFICE_H
#define INCLUDED_DESKTOP_INC_LIBLIBREOFFICE_H

#ifdef __cplusplus
extern "C"
{
#endif

typedef struct _LibreOffice LibreOffice;
typedef struct _LibreOfficeDocument LibreOfficeDocument;

struct _LibreOffice
{
  int  nSize;

  void                 (*destroy)       (LibreOffice *pThis);
  int                  (*initialize)    (LibreOffice *pThis, const char *pInstallPath);
  LibreOfficeDocument* (*documentLoad)  (LibreOffice *pThis, const char *pURL);
  char*                (*getError)      (LibreOffice *pThis);
};

#ifdef LLO_USE_UNSTABLE_API
typedef enum
{
    WRITER,
    SPREADSHEET,
    PRESENTATION,
    OTHER
}
LibreOfficeDocumentType;
#endif // LLO_USE_UNSTABLE_API

struct _LibreOfficeDocument
{
  int  nSize;

  void (*destroy)   (LibreOfficeDocument* pThis);
  int (*saveAs)     (LibreOfficeDocument* pThis,
                     const char *pUrl,
                     const char *pFormat);
  int (*saveAsWithOptions) (LibreOfficeDocument* pThis,
                            const char *pUrl,
                            const char *pFormat,
                            const char *pFilterOptions);

#ifdef LLO_USE_UNSTABLE_API
  LibreOfficeDocumentType (*getDocumentType)    (LibreOfficeDocument* pThis);

  // Part refers to either indivual sheets in a Spreadsheet, or slides
  // in a Slideshow, and has no relevance for wrtier documents.
  int (*getNumberOfParts)    (LibreOfficeDocument* pThis);

  void (*setPart)            (LibreOfficeDocument* pThis,
                              int nPart);

  // Get a pointer to a raw array, of size 3*nCanvasWidth*nCanvasHeight
  // Basebmp's bitmap device seems to round the width up if needed
  // for its internal buffer, i.e. the rowstride for the buffer may be larger
  // than the desired width, hence we need to be able to return the
  // rowstride too.
  unsigned char* (*paintTile)  (LibreOfficeDocument* pThis,
                     const int nCanvasWidth,
                     const int nCanvasHeight,
                     int* pRowStride,
                     const int nTilePosX,
                     const int nTilePosY,
                     const int nTileWidth,
                     const int nTileHeight);
#endif // LLO_USE_UNSTABLE_API
};

LibreOffice* lo_init (const char* pInstallPath);

#ifdef __cplusplus
}
#endif

#endif
/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
