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
#include "State/FrameStack/FunctionLayer.h"
#include "Xml/TypeFilter.h"

namespace Jam
{
    class FrameStack;
}

namespace Jam::Editor::State
{
    class RuntimeProjectState;
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

    enum ProjectFileTags
    {
        ProjectFileStart = ReserveSpace(ProjectOrder),
        JamProjectTag,
        ProjectFileMax
    };

    enum LayoutTreeTags
    {
        LayoutTreeStart = ReserveSpace(LayoutOrder),
        TreeTag,
        BranchTag,
        LeafTag,
        LayoutTreeMax
    };

    enum FrameStackTags
    {
        FrameStackStart = ReserveSpace(FrameStackOrder),
        FrameStackTag,
        GridTag,
        FunctionTag,
        FrameStackMax
    };

    constexpr size_t FrameStackTagsMax  = (FrameStackMax - FrameStackStart) - 1;
    constexpr size_t AreaLayoutTagsMax  = (LayoutTreeMax - LayoutTreeStart) - 1;
    constexpr size_t ProjectFileTagsMax = FrameStackTagsMax +
                                          AreaLayoutTagsMax +
                                          (ProjectFileMax - ProjectFileStart) - 1;

    constexpr TypeFilter FrameStackTags[FrameStackTagsMax] = {
        {   "stack", FrameStackTag}, // FrameStackTags
        {    "grid",       GridTag},
        {"function",   FunctionTag},
    };

    constexpr TypeFilter AreaLayoutTags[AreaLayoutTagsMax] = {
        {  "tree",   TreeTag}, // AreaLayoutTags
        {  "leaf",   LeafTag},
        {"branch", BranchTag},
    };

    constexpr TypeFilter ProjectFileTags[ProjectFileTagsMax] = {
        {     "jam", JamProjectTag}, // ProjectFileTags
        {    "tree",       TreeTag}, // AreaLayoutTags
        {    "leaf",       LeafTag},
        {  "branch",     BranchTag},
        {   "stack", FrameStackTag}, // FrameStackTags
        {    "grid",       GridTag},
        {"function",   FunctionTag},
    };


}  // namespace Jam::Editor::State
