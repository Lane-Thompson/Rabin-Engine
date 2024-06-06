#pragma once

// Include all node headers in this file

// Example Control Flow Nodes
#include "ControlFlow/C_ParallelSequencer.h"
#include "ControlFlow/C_RandomSelector.h"
#include "ControlFlow/C_Selector.h"
#include "ControlFlow/C_Sequencer.h"

// Student Control Flow Nodes


// Example Decorator Nodes
#include "Decorator/D_Delay.h"
#include "Decorator/D_InvertedRepeater.h"
#include "Decorator/D_RepeatFourTimes.h"

// Student Decorator Nodes
#include "Decorator/D_InvertResult.h"
#include "Decorator/D_EnoughTrees.h"
#include "Decorator/D_IsValidTarget.h"
#include "Decorator/D_AlwaysSuccess.h"
#include "Decorator/D_ChoppedEnough.h"


// Example Leaf Nodes
#include "Leaf/L_CheckMouseClick.h"
#include "Leaf/L_Idle.h"
#include "Leaf/L_MoveToFurthestAgent.h"
#include "Leaf/L_MoveToMouseClick.h"
#include "Leaf/L_MoveToRandomPosition.h"
#include "Leaf/L_PlaySound.h"

// Student Leaf Nodes
#include "Leaf/L_Grow.h"
#include "Leaf/L_Age.h"
#include "Leaf/L_TreeDeath.h"
#include "Leaf/L_DestroyAgent.h"
#include "Leaf/L_DuplicateTree.h"
#include "Leaf/L_MoveHome.h"
#include "Leaf/L_SetNearestTarget.h"
#include "Leaf/L_MoveToTarget.h"
#include "Leaf/L_Disappear.h"
#include "Leaf/L_Appear.h"
#include "Leaf/L_ChopTree.h"
#include "Leaf/L_SpawnLumberjack.h"
#include "Leaf/L_SetRandomTarget.h"
#include "Leaf/L_GetRandomPosition.h"
#include "Leaf/L_MoveToTargetPosition.h"