#ifndef NUIE_UNDOHANDLER_HPP
#define NUIE_UNDOHANDLER_HPP

#include "../NodeEngine/NE_NodeManager.hpp"
#include "../NodeEngine/NE_NodeManagerMerge.hpp"

#include <vector>

namespace NUIE
{

class UndoHandler
{
public:
	enum class ChangeResult
	{
		Changed,
		NotChanged
	};

	UndoHandler ();

	bool			CanUndo () const;
	bool			CanRedo () const;

	ChangeResult	AddUndoStep (const NE::NodeManager& nodeManager);

	ChangeResult	Undo (NE::NodeManager& targetNodeManager, NE::UpdateEventHandler& eventHandler);
	ChangeResult	Redo (NE::NodeManager& targetNodeManager, NE::UpdateEventHandler& eventHandler);

	ChangeResult	Clear ();

private:
	std::vector<std::shared_ptr<NE::NodeManager>>	undoStack;
	std::vector<std::shared_ptr<NE::NodeManager>>	redoStack;
};

}

#endif
