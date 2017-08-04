/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Tencent is pleased to support the open source community by making behaviac available.
//
// Copyright (C) 2015 THL A29 Limited, a Tencent company. All rights reserved.
//
// Licensed under the BSD 3-Clause License (the "License"); you may not use this file except in compliance with
// the License. You may obtain a copy of the License at http://opensource.org/licenses/BSD-3-Clause
//
// Unless required by applicable law or agreed to in writing, software distributed under the License is
// distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and limitations under the License.
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef BEHAVIAC_BEHAVIORTREE_DECORATORFRAMES_H
#define BEHAVIAC_BEHAVIORTREE_DECORATORFRAMES_H

#include "behaviac/base/base.h"
#include "behaviac/behaviortree/behaviortree.h"
#include "behaviac/behaviortree/behaviortree_task.h"

namespace behaviac
{
    /*! \addtogroup treeNodes Behavior Tree
    * @{
    * \addtogroup DecoratorFrames
    * @{ */

    /**
    It returns Running result until it reaches the frame count specified, no matter which
    value its child return. Or return the child's value.
    */
    class BEHAVIAC_API DecoratorFrames : public DecoratorNode
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorFrames, DecoratorNode);

        DecoratorFrames();
        virtual ~DecoratorFrames();

        virtual void load(int version, const char* agentType, const properties_t& properties);

        virtual int GetFrames(Agent* pAgent) const;

    private:
        virtual BehaviorTask* createTask() const;

    protected:
        Property*		m_frames_var;
		behaviac::CMethodBase*	m_frames_method;

        friend class DecoratorFramesTask;
    };

    class BEHAVIAC_API DecoratorFramesTask : public DecoratorTask
    {
    public:
        BEHAVIAC_DECLARE_DYNAMIC_TYPE(DecoratorFramesTask, DecoratorTask);

        DecoratorFramesTask();

    protected:
        virtual ~DecoratorFramesTask();

        virtual void copyto(BehaviorTask* target) const;
        virtual void save(ISerializableNode* node) const;
        virtual void load(ISerializableNode* node);

        virtual bool onenter(Agent* pAgent);
        virtual EBTStatus decorate(EBTStatus status);

        int GetFrames(Agent* pAgent) const;

    private:
        int	m_start;
        int	m_frames;
    };
    /*! @} */
    /*! @} */
}

#endif//BEHAVIAC_BEHAVIORTREE_DECORATORFRAMES_H
