/* 
 * 
 * Copyright (c) 1997  Metro Link Incorporated
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"), 
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE X CONSORTIUM BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 * WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
 * OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 * Except as contained in this notice, the name of the Metro Link shall not be
 * used in advertising or otherwise to promote the sale, use or other dealings
 * in this Software without prior written authorization from Metro Link.
 * 
 */
/*
 * Copyright (c) 1997-2003 by The XFree86 Project, Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE COPYRIGHT HOLDER(S) OR AUTHOR(S) BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 *
 * Except as contained in this notice, the name of the copyright holder(s)
 * and author(s) shall not be used in advertising or otherwise to promote
 * the sale, use or other dealings in this Software without prior written
 * authorization from the copyright holder(s) and author(s).
 */


/* View/edit this file with tab stops set to 4 */

#include "xf86Parser.h"
#include "xf86tokens.h"
#include "Configint.h"

extern LexRec val;

static XConfigSymTabRec DisplayTab[] =
{
    {ENDSUBSECTION, "endsubsection"},
    {MODES, "modes"},
    {VIEWPORT, "viewport"},
    {VIRTUAL, "virtual"},
    {VISUAL, "visual"},
    {BLACK_TOK, "black"},
    {WHITE_TOK, "white"},
    {DEPTH, "depth"},
    {BPP, "fbbpp"},
    {WEIGHT, "weight"},
    {OPTION, "option"},
    {-1, ""},
};

#define CLEANUP xconfigFreeDisplayList

XConfigDisplayPtr
xconfigParseDisplaySubSection (void)
{
    int token;
    PARSE_PROLOGUE (XConfigDisplayPtr, XConfigDisplayRec)

    ptr->black.red = ptr->black.green = ptr->black.blue = -1;
    ptr->white.red = ptr->white.green = ptr->white.blue = -1;
    ptr->frameX0 = ptr->frameY0 = -1;
    while ((token = xconfigGetToken (DisplayTab)) != ENDSUBSECTION)
    {
        switch (token)
        {
        case COMMENT:
            ptr->comment = xconfigAddComment(ptr->comment, val.str);
            break;
        case VIEWPORT:
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (VIEWPORT_MSG, NULL);
            ptr->frameX0 = val.num;
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (VIEWPORT_MSG, NULL);
            ptr->frameY0 = val.num;
            break;
        case VIRTUAL:
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (VIRTUAL_MSG, NULL);
            ptr->virtualX = val.num;
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (VIRTUAL_MSG, NULL);
            ptr->virtualY = val.num;
            break;
        case DEPTH:
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (NUMBER_MSG, "Display");
            ptr->depth = val.num;
            break;
        case BPP:
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (NUMBER_MSG, "Display");
            ptr->bpp = val.num;
            break;
        case VISUAL:
            if (xconfigGetSubToken (&(ptr->comment)) != STRING)
                Error (QUOTE_MSG, "Display");
            ptr->visual = val.str;
            break;
        case WEIGHT:
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (WEIGHT_MSG, NULL);
            ptr->weight.red = val.num;
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (WEIGHT_MSG, NULL);
            ptr->weight.green = val.num;
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (WEIGHT_MSG, NULL);
            ptr->weight.blue = val.num;
            break;
        case BLACK_TOK:
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (BLACK_MSG, NULL);
            ptr->black.red = val.num;
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (BLACK_MSG, NULL);
            ptr->black.green = val.num;
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (BLACK_MSG, NULL);
            ptr->black.blue = val.num;
            break;
        case WHITE_TOK:
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (WHITE_MSG, NULL);
            ptr->white.red = val.num;
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (WHITE_MSG, NULL);
            ptr->white.green = val.num;
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (WHITE_MSG, NULL);
            ptr->white.blue = val.num;
            break;
        case MODES:
            {
                XConfigModePtr mptr;

                while ((token =
                        xconfigGetSubTokenWithTab(&(ptr->comment),
                                                  DisplayTab)) == STRING)
                {
                    mptr = calloc (1, sizeof (XConfigModeRec));
                    mptr->mode_name = val.str;
                    mptr->next = NULL;
                    ptr->modes = (XConfigModePtr)
                        xconfigAddListItem ((GenericListPtr) ptr->modes,
                                         (GenericListPtr) mptr);
                }
                xconfigUnGetToken (token);
            }
            break;
        case OPTION:
            ptr->options = xconfigParseOption(ptr->options);
            break;
            
        case EOF_TOKEN:
            Error (UNEXPECTED_EOF_MSG, NULL);
            break;
        default:
            Error (INVALID_KEYWORD_MSG, xconfigTokenString ());
            break;
        }
    }

#ifdef DEBUG
    xconfigErrorMsg(DebugMsg, "Display subsection parsed\n");
#endif

    return ptr;
}

#undef CLEANUP

static XConfigSymTabRec ScreenTab[] =
{
    {ENDSECTION, "endsection"},
    {IDENTIFIER, "identifier"},
    {OBSDRIVER, "driver"},
    {MDEVICE, "device"},
    {MONITOR, "monitor"},
    {VIDEOADAPTOR, "videoadaptor"},
    {SCREENNO, "screenno"},
    {SUBSECTION, "subsection"},
    {DEFAULTDEPTH, "defaultcolordepth"},
    {DEFAULTDEPTH, "defaultdepth"},
    {DEFAULTBPP, "defaultbpp"},
    {DEFAULTFBBPP, "defaultfbbpp"},
    {OPTION, "option"},
    {-1, ""},
};

#define CLEANUP xconfigFreeScreenList
XConfigScreenPtr
xconfigParseScreenSection (void)
{
    int has_ident = FALSE;
    int has_driver= FALSE;
    int token;

    PARSE_PROLOGUE (XConfigScreenPtr, XConfigScreenRec)

        while ((token = xconfigGetToken (ScreenTab)) != ENDSECTION)
    {
        switch (token)
        {
        case COMMENT:
            ptr->comment = xconfigAddComment(ptr->comment, val.str);
            break;
        case IDENTIFIER:
            if (xconfigGetSubToken (&(ptr->comment)) != STRING)
                Error (QUOTE_MSG, "Identifier");
            ptr->identifier = val.str;
            if (has_ident || has_driver)
                Error (ONLY_ONE_MSG,"Identifier or Driver");
            has_ident = TRUE;
            break;
        case OBSDRIVER:
            if (xconfigGetSubToken (&(ptr->comment)) != STRING)
                Error (QUOTE_MSG, "Driver");
            ptr->obsolete_driver = val.str;
            if (has_ident || has_driver)
                Error (ONLY_ONE_MSG,"Identifier or Driver");
            has_driver = TRUE;
            break;
        case DEFAULTDEPTH:
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (NUMBER_MSG, "DefaultDepth");
            ptr->defaultdepth = val.num;
            break;
        case DEFAULTBPP:
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (NUMBER_MSG, "DefaultBPP");
            ptr->defaultbpp = val.num;
            break;
        case DEFAULTFBBPP:
            if (xconfigGetSubToken (&(ptr->comment)) != NUMBER)
                Error (NUMBER_MSG, "DefaultFbBPP");
            ptr->defaultfbbpp = val.num;
            break;
        case MDEVICE:
            if (xconfigGetSubToken (&(ptr->comment)) != STRING)
                Error (QUOTE_MSG, "Device");
            ptr->device_name = val.str;
            break;
        case MONITOR:
            if (xconfigGetSubToken (&(ptr->comment)) != STRING)
                Error (QUOTE_MSG, "Monitor");
            ptr->monitor_name = val.str;
            break;
        case VIDEOADAPTOR:
            {
                XConfigAdaptorLinkPtr aptr;

                if (xconfigGetSubToken (&(ptr->comment)) != STRING)
                    Error (QUOTE_MSG, "VideoAdaptor");

                /* Don't allow duplicates */
                for (aptr = ptr->adaptors; aptr; 
                    aptr = (XConfigAdaptorLinkPtr) aptr->next)
                    if (xconfigNameCompare (val.str, aptr->adaptor_name) == 0)
                        break;

                if (aptr == NULL)
                {
                    aptr = calloc (1, sizeof (XConfigAdaptorLinkRec));
                    aptr->next = NULL;
                    aptr->adaptor_name = val.str;
                    ptr->adaptors = (XConfigAdaptorLinkPtr)
                        xconfigAddListItem ((GenericListPtr) ptr->adaptors,
                                         (GenericListPtr) aptr);
                }
            }
            break;
        case OPTION:
            ptr->options = xconfigParseOption(ptr->options);
            break;
        case SUBSECTION:
            if (xconfigGetSubToken (&(ptr->comment)) != STRING)
                Error (QUOTE_MSG, "SubSection");
            {
                free(val.str);
                HANDLE_LIST (displays, xconfigParseDisplaySubSection,
                             XConfigDisplayPtr);
            }
            break;
        case EOF_TOKEN:
            Error (UNEXPECTED_EOF_MSG, NULL);
            break;
        default:
            Error (INVALID_KEYWORD_MSG, xconfigTokenString ());
            break;
        }
    }

    if (!has_ident && !has_driver)
        Error (NO_IDENT_MSG, NULL);

#ifdef DEBUG
    xconfigErrorMsg(DebugMsg, "Screen section parsed\n");
#endif

    return ptr;
}

void
xconfigPrintScreenSection (FILE * cf, XConfigScreenPtr ptr)
{
    XConfigAdaptorLinkPtr aptr;
    XConfigDisplayPtr dptr;
    XConfigModePtr mptr;

    while (ptr)
    {
        fprintf (cf, "Section \"Screen\"\n");
        if (ptr->comment)
            fprintf (cf, "%s", ptr->comment);
        if (ptr->identifier)
            fprintf (cf, "    Identifier     \"%s\"\n", ptr->identifier);
        if (ptr->obsolete_driver)
            fprintf (cf, "    Driver         \"%s\"\n", ptr->obsolete_driver);
        if (ptr->device_name)
            fprintf (cf, "    Device         \"%s\"\n", ptr->device_name);
        if (ptr->monitor_name)
            fprintf (cf, "    Monitor        \"%s\"\n", ptr->monitor_name);
        if (ptr->defaultdepth)
            fprintf (cf, "    DefaultDepth    %d\n",
                     ptr->defaultdepth);
        if (ptr->defaultbpp)
            fprintf (cf, "    DefaultBPP      %d\n",
                     ptr->defaultbpp);
        if (ptr->defaultfbbpp)
            fprintf (cf, "    DefaultFbBPP    %d\n",
                     ptr->defaultfbbpp);
        xconfigPrintOptionList(cf, ptr->options, 1);
        for (aptr = ptr->adaptors; aptr; aptr = aptr->next)
        {
            fprintf (cf, "    VideoAdaptor   \"%s\"\n", aptr->adaptor_name);
        }
        for (dptr = ptr->displays; dptr; dptr = dptr->next)
        {
            fprintf (cf, "    SubSection     \"Display\"\n");
            if (dptr->comment)
                fprintf (cf, "%s", dptr->comment);
            if (dptr->frameX0 >= 0 || dptr->frameY0 >= 0)
            {
                fprintf (cf, "        Viewport    %d %d\n",
                         dptr->frameX0, dptr->frameY0);
            }
            if (dptr->virtualX != 0 || dptr->virtualY != 0)
            {
                fprintf (cf, "        Virtual     %d %d\n",
                         dptr->virtualX, dptr->virtualY);
            }
            if (dptr->depth)
            {
                fprintf (cf, "        Depth       %d\n", dptr->depth);
            }
            if (dptr->bpp)
            {
                fprintf (cf, "        FbBPP       %d\n", dptr->bpp);
            }
            if (dptr->visual)
            {
                fprintf (cf, "        Visual     \"%s\"\n", dptr->visual);
            }
            if (dptr->weight.red != 0)
            {
                fprintf (cf, "        Weight      %d %d %d\n",
                     dptr->weight.red, dptr->weight.green, dptr->weight.blue);
            }
            if (dptr->black.red != -1)
            {
                fprintf (cf, "        Black       0x%04x 0x%04x 0x%04x\n",
                      dptr->black.red, dptr->black.green, dptr->black.blue);
            }
            if (dptr->white.red != -1)
            {
                fprintf (cf, "        White       0x%04x 0x%04x 0x%04x\n",
                      dptr->white.red, dptr->white.green, dptr->white.blue);
            }
            if (dptr->modes)
            {
                fprintf (cf, "        Modes     ");
            }
            for (mptr = dptr->modes; mptr; mptr = mptr->next)
            {
                fprintf (cf, " \"%s\"", mptr->mode_name);
            }
            if (dptr->modes)
            {
                fprintf (cf, "\n");
            }
            xconfigPrintOptionList(cf, dptr->options, 2);
            fprintf (cf, "    EndSubSection\n");
        }
        fprintf (cf, "EndSection\n\n");
        ptr = ptr->next;
    }

}

void
xconfigFreeScreenList (XConfigScreenPtr ptr)
{
    XConfigScreenPtr prev;

    while (ptr)
    {
        TEST_FREE (ptr->identifier);
        TEST_FREE (ptr->monitor_name);
        TEST_FREE (ptr->device_name);
        TEST_FREE (ptr->comment);
        xconfigOptionListFree (ptr->options);
        xconfigFreeAdaptorLinkList (ptr->adaptors);
        xconfigFreeDisplayList (ptr->displays);
        prev = ptr;
        ptr = ptr->next;
        free (prev);
    }
}

void
xconfigFreeAdaptorLinkList (XConfigAdaptorLinkPtr ptr)
{
    XConfigAdaptorLinkPtr prev;

    while (ptr)
    {
        TEST_FREE (ptr->adaptor_name);
        prev = ptr;
        ptr = ptr->next;
        free (prev);
    }
}

void
xconfigFreeDisplayList (XConfigDisplayPtr ptr)
{
    XConfigDisplayPtr prev;

    while (ptr)
    {
        xconfigFreeModeList (ptr->modes);
        xconfigOptionListFree (ptr->options);
        prev = ptr;
        ptr = ptr->next;
        free (prev);
    }
}

void
xconfigFreeModeList (XConfigModePtr ptr)
{
    XConfigModePtr prev;

    while (ptr)
    {
        TEST_FREE (ptr->mode_name);
        prev = ptr;
        ptr = ptr->next;
        free (prev);
    }
}

int
xconfigValidateScreen (XConfigPtr p)
{
    XConfigScreenPtr screen = p->screens;
    XConfigMonitorPtr monitor;
    XConfigDevicePtr device;
    XConfigAdaptorLinkPtr adaptor;

    if (!screen)
    {
        xconfigErrorMsg(ValidationErrorMsg, "At least one Screen section "
                     "is required.");
        return (FALSE);
    }

    while (screen)
    {
        if (screen->obsolete_driver && !screen->identifier)
            screen->identifier = screen->obsolete_driver;

        monitor = xconfigFindMonitor (screen->monitor_name, p->monitors);
        if (screen->monitor_name)
        {
            if (!monitor)
            {
                xconfigErrorMsg(ValidationErrorMsg, UNDEFINED_MONITOR_MSG,
                             screen->monitor_name, screen->identifier);
                return (FALSE);
            }
            else
            {
                screen->monitor = monitor;
                if (!xconfigValidateMonitor(p, screen))
                    return (FALSE);
            }
        }

        device = xconfigFindDevice (screen->device_name, p->devices);
        if (!device)
        {
            xconfigErrorMsg(ValidationErrorMsg, UNDEFINED_DEVICE_MSG,
                         screen->device_name, screen->identifier);
            return (FALSE);
        }
        else
            screen->device = device;

        adaptor = screen->adaptors;
        while (adaptor) {
            adaptor->adaptor = xconfigFindVideoAdaptor(adaptor->adaptor_name,
                                                       p->videoadaptors);
            if (!adaptor->adaptor) {
                xconfigErrorMsg(ValidationErrorMsg, UNDEFINED_ADAPTOR_MSG,
                             adaptor->adaptor_name,
                             screen->identifier);
                return (FALSE);
            } else if (adaptor->adaptor->fwdref) {
                xconfigErrorMsg(ValidationErrorMsg, ADAPTOR_REF_TWICE_MSG,
                             adaptor->adaptor_name,
                             adaptor->adaptor->fwdref);
                return (FALSE);
            }
            
            adaptor->adaptor->fwdref = xconfigStrdup(screen->identifier);
            adaptor = adaptor->next;
        }

        screen = screen->next;
    }

    return (TRUE);
}

XConfigScreenPtr
xconfigFindScreen (const char *ident, XConfigScreenPtr p)
{
    while (p)
    {
        if (xconfigNameCompare (ident, p->identifier) == 0)
            return (p);

        p = p->next;
    }
    return (NULL);
}


XConfigModePtr
xconfigAddMode(XConfigModePtr head, const char *name)
{
    XConfigModePtr mode;
    
    mode = xconfigAlloc(sizeof(XConfigModeRec));
    mode->mode_name = xconfigStrdup(name);

    mode->next = head;

    return mode;

}


XConfigModePtr
xconfigRemoveMode(XConfigModePtr head, const char *name)
{
    XConfigModePtr prev = NULL;
    XConfigModePtr m = head;

    while (m) {
        if (xconfigNameCompare(m->mode_name, name) == 0) {
            if (prev) prev->next = m->next;
            if (head == m) head = m->next;
            free(m->mode_name);
            free(m);
            break;
        }
        prev = m;
        m = m->next;
    }

    return head;
}



