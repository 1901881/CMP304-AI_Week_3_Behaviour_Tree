#include <LeafNode.h>
#include <FallbackSelector.h>
#include <SequenceSelector.h>
#include <Decorator.h>
#include <Blackboard.h>
#include <iostream>
#include <conio.h>
#include <thread>
#include <chrono>

using namespace BT;

BT::NodeStatus leafFunction(BT::TreeNode* running_node)
{
	std::cout << "This leaf node is being run. " << std::endl;

	return BT::NodeStatus::NODE_RUNNING;
}

BT::NodeStatus leafAlarmFunction(BT::TreeNode* owner)
{
	std::cout << "Raising Alarm. " << std::endl;

	return BT::NodeStatus::NODE_FAILURE;
}

BT::NodeStatus leafStandFunction(BT::TreeNode* owner)
{
	if (Blackboard::getInstance()->getAndDeleteValue("disturbance"))
	{
		return BT::NodeStatus::NODE_FAILURE;
	}
	
	std::cout << "Standing Still.......... " << std::endl;

	return BT::NodeStatus::NODE_RUNNING;
}

BT::NodeStatus leafPatrolFunction(BT::TreeNode* owner)
{
	if (Blackboard::getInstance()->getAndDeleteValue("disturbance"))
	{
		return BT::NodeStatus::NODE_FAILURE;
	}

	std::cout << "Patrolling......... " << std::endl;

	return BT::NodeStatus::NODE_RUNNING;
}

BT::NodeStatus leafDisturbFunction(BT::TreeNode* owner)
{
	if (Blackboard::getInstance()->getAndDeleteValue("disturbance"))
	{
		return BT::NodeStatus::NODE_FAILURE;
	}

	std::cout << "Moving Towards Disturbance......... " << std::endl;

	return BT::NodeStatus::NODE_RUNNING;
}

BT::NodeStatus leafExploreFunction(BT::TreeNode* owner)
{
	if (Blackboard::getInstance()->getAndDeleteValue("disturbance"))
	{
		return BT::NodeStatus::NODE_FAILURE;
	}

	std::cout << "Exploring near disturbance......... " << std::endl;

	return BT::NodeStatus::NODE_RUNNING;
}

int main(int argc, char** argv)
{

	//BT::PriotitySelector branch1("branch");

	//PriotitySelector branch1("branch");

	FallbackSelector root("rootNode");

	FallbackSelector branch1("mainBranch");
	LeafNode leafFive("Alarm", leafAlarmFunction);

	root.addChildNode(&branch1);
	root.addChildNode(&leafFive);

	SequenceSelector guardBranch("guard");
	SequenceSelector exploreBranch("explore");

	branch1.addChildNode(&guardBranch);
	branch1.addChildNode(&exploreBranch);

	Decorator leafDecorOne("timerInsert", 10);
	Decorator leafDecorTwo("timerInsert", 20);
	Decorator leafDecorThree("timerInsert", 5);
	Decorator leafDecorFour("timerInsert", 15);

	guardBranch.addChildNode(&leafDecorOne);
	guardBranch.addChildNode(&leafDecorTwo);
	exploreBranch.addChildNode(&leafDecorThree);
	exploreBranch.addChildNode(&leafDecorFour);

	LeafNode leafOne("Standing", leafStandFunction);
	LeafNode leafTwo("Patrolling", leafPatrolFunction);
	LeafNode leafThree("Disturbance", leafDisturbFunction);
	LeafNode leafFour("Explore", leafExploreFunction);


	leafDecorOne.addChildNode(&leafOne);
	leafDecorTwo.addChildNode(&leafTwo);
	leafDecorThree.addChildNode(&leafThree);
	leafDecorFour.addChildNode(&leafFour);


	while(true)
	{
		if (_kbhit())
		{
			_getch();

			std::cout << "Guard hears a disturbance!" << std::endl;
			Blackboard::getInstance()->addNewValue("disturbance", 1);
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
		}
		/*
		add remaining code here
		*/

		NodeStatus result = root.tick();

		if (result == NodeStatus::NODE_SUCCESS)
		{
			root.reset();
		}
		else if (result == NodeStatus::NODE_FAILURE)
		{
			break;
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(100));
	}





	return 0;
}