/*
-------------------------------------------------------------------------------
    Copyright (c) Charles Carley.

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.
-------------------------------------------------------------------------------
*/
#pragma once
#include "Xml/TypeFilter.h"

namespace Jam::Editor::Tags
{
    constexpr U32 Reserved = 0x3E5;

    constexpr size_t ReserveSpace(const size_t mul)
    {
        return mul * Reserved;
    }

    enum TagOrder
    {
        ProjectOrder = 1,
        LayoutOrder,
        FrameStackOrder,
    };

    enum ProjectFileIds
    {
        ProjectFileStart = ReserveSpace(ProjectOrder),
        JamTagId,
        ProjectFileIdMax
    };

    enum LayoutTreeIds
    {
        LayoutTreeStart = ReserveSpace(LayoutOrder),
        TreeTagId,
        BranchTagId,
        LeafTagId,
        LayoutTreeMax
    };

    enum FrameStackIds
    {
        FrameStackStart = ReserveSpace(FrameStackOrder),
        FrameStackTagId,
        GridTagId,
        FunctionTagId,
        VariableTagId,
        ExpressionTagId,
        FrameStackMax
    };

    constexpr size_t FrameStackTagsMax  = (FrameStackMax - FrameStackStart) - 1;
    constexpr size_t AreaLayoutTagsMax  = (LayoutTreeMax - LayoutTreeStart) - 1;
    constexpr size_t ProjectFileTagsMax = FrameStackTagsMax +
                                          AreaLayoutTagsMax +
                                          (ProjectFileIdMax - ProjectFileStart) - 1;

    constexpr TypeFilter FrameStackTag = {"stack", FrameStackTagId};
    constexpr TypeFilter GridTag       = {"grid", GridTagId};
    constexpr TypeFilter FunctionTag   = {"func", FunctionTagId};
    constexpr TypeFilter VariableTag   = {"var", VariableTagId};
    constexpr TypeFilter ExpressionTag = {"expr", ExpressionTagId};

    constexpr TypeFilter FrameStackTags[FrameStackTagsMax] = {
        FrameStackTag,
        GridTag,
        FunctionTag,
        VariableTag,
        ExpressionTag,
    };

    constexpr TypeFilter TreeTag   = {"tree", TreeTagId};
    constexpr TypeFilter LeafTag   = {"leaf", LeafTagId};
    constexpr TypeFilter BranchTag = {"branch", BranchTagId};

    constexpr TypeFilter AreaLayoutTags[AreaLayoutTagsMax] = {
        TreeTag,
        LeafTag,
        BranchTag,
    };

    constexpr TypeFilter JamTag = {"jam", JamTagId};

    constexpr TypeFilter ProjectFileTags[ProjectFileTagsMax] = {
        JamTag,
        TreeTag,
        LeafTag,
        BranchTag,
        FrameStackTag,
        GridTag,
        FunctionTag,
        VariableTag,
        ExpressionTag,
    };

}  // namespace Jam::Editor::Tags
