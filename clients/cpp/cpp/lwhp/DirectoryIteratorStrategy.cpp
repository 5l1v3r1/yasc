













#include "Poco/DirectoryIteratorStrategy.h"


namespace Poco {





TraverseBase::TraverseBase(DepthFun depthDeterminer, UInt16 maxDepth)
	: _depthDeterminer(depthDeterminer), _maxDepth(maxDepth)
{
}


inline bool TraverseBase::isFiniteDepth()
{
	return _maxDepth != D_INFINITE;
}


bool TraverseBase::isDirectory(Poco::File& file)
{
	try
	{
		return file.isDirectory();
	}
	catch (...)
	{
		return false;
	}
}





ChildrenFirstTraverse::ChildrenFirstTraverse(DepthFun depthDeterminer, UInt16 maxDepth)
	: TraverseBase(depthDeterminer, maxDepth)
{
}


const std::string ChildrenFirstTraverse::next(Stack* itStack, bool* isFinished)
{
	
	poco_check_ptr(isFinished);
	poco_assert(!(*isFinished));

	std::stack<DirectoryIterator> it;

	

	
	
	bool isDepthLimitReached = isFiniteDepth() && _depthDeterminer(*itStack) >= _maxDepth;
	if (!isDepthLimitReached && isDirectory(*itStack->top()))
	{
		DirectoryIterator child_it(itStack->top().path());
		
		if (child_it != _itEnd)
		{
			itStack->push(child_it);
			return child_it->path();
		}
	}

	++(itStack->top());

	poco_assert(!itStack->empty());
	
	while (itStack->top() == _itEnd)
	{
		itStack->pop();

		
		if (itStack->empty())
		{
			*isFinished = true;
			return _itEnd->path();
		}
		else
		{
			++(itStack->top());
		}
	}

	return itStack->top()->path();
}





SiblingsFirstTraverse::SiblingsFirstTraverse(DepthFun depthDeterminer, UInt16 maxDepth)
	: TraverseBase(depthDeterminer, maxDepth)
{
	_dirsStack.push(std::queue<std::string>());
}


const std::string SiblingsFirstTraverse::next(Stack* itStack, bool* isFinished)
{
	
	poco_check_ptr(isFinished);
	poco_assert(!(*isFinished));

	
	bool isDepthLimitReached = isFiniteDepth() && _depthDeterminer(*itStack) >= _maxDepth;
	if (!isDepthLimitReached && isDirectory(*itStack->top()))
	{
		const std::string& p = itStack->top()->path();
		_dirsStack.top().push(p);
	}

	++(itStack->top());

	poco_assert(!itStack->empty());
	
	while (itStack->top() == _itEnd)
	{
		
		while (!_dirsStack.top().empty())
		{
			std::string dir = _dirsStack.top().front();
			_dirsStack.top().pop();
			DirectoryIterator child_it(dir);

			
			if (child_it != _itEnd)
			{
				itStack->push(child_it);
				_dirsStack.push(std::queue<std::string>());
				return child_it->path();
			}
		}

		
		itStack->pop();
		_dirsStack.pop();

		
		if (itStack->empty())
		{
			*isFinished = true;
			return _itEnd->path();
		}
	}

	return itStack->top()->path();
}


} 
