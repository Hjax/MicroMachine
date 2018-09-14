#include "BuildOrderQueue.h"
#include "CCBot.h"

BuildOrderQueue::BuildOrderQueue(CCBot & bot)
    : m_bot(bot)
    , m_highestPriority(0)
    , m_lowestPriority(0)
    , m_defaultPrioritySpacing(10)
    , m_numSkippedItems(0)
{

}

void BuildOrderQueue::clearAll()
{
    // clear the queue
    m_queue.clear();

    // reset the priorities
    m_highestPriority = 0;
    m_lowestPriority = 0;
}

BuildOrderItem BuildOrderQueue::getHighestPriorityItem()
{
    // reset the number of skipped items to zero
    m_numSkippedItems = 0;

    // the queue will be sorted with the highest priority at the back
    return m_queue.back();
}

BuildOrderItem BuildOrderQueue::getNextHighestPriorityItem()
{
    assert(m_queue.size() - 1 - m_numSkippedItems >= 0);

    // the queue will be sorted with the highest priority at the back
    return m_queue[m_queue.size() - 1 - m_numSkippedItems];
}

int BuildOrderQueue::getCountOfType(const MetaType & type)
{
	int count = 0;
	for (auto item : m_queue)
	{
		auto a = item.type.getName();
		auto b = type.getName();
		if (item.type.getUnitType() == type.getUnitType())
		{
			count++;
		}
	}
	return count;
}

void BuildOrderQueue::skipItem()
{
    // make sure we can skip
    assert(canSkipItem());

    // skip it
    m_numSkippedItems++;
}

bool BuildOrderQueue::canSkipItem()
{
    // does the queue have more elements
    bool bigEnough = m_queue.size() > (size_t)(1 + m_numSkippedItems);

    if (!bigEnough)
    {
        return false;
    }

    // is the current highest priority item not blocking a skip
    bool highestNotBlocking = !m_queue[m_queue.size() - 1 - m_numSkippedItems].blocking;

    // this tells us if we can skip
    return highestNotBlocking;
}

void BuildOrderQueue::queueItem(const BuildOrderItem & b)
{
    // if the queue is empty, set the highest and lowest priorities
    if (m_queue.empty())
    {
        m_highestPriority = b.priority;
        m_lowestPriority = b.priority;
    }

    // push the item into the queue
    if (b.priority <= m_lowestPriority)
    {
        m_queue.push_front(b);
    }
    else
    {
        m_queue.push_back(b);
    }

    // if the item is somewhere in the middle, we have to sort again
    if ((m_queue.size() > 1) && (b.priority < m_highestPriority) && (b.priority > m_lowestPriority))
    {
        // sort the list in ascending order, putting highest priority at the top
        std::sort(m_queue.begin(), m_queue.end());
    }

    // update the highest or lowest if it is beaten
    m_highestPriority = (b.priority > m_highestPriority) ? b.priority : m_highestPriority;
    m_lowestPriority  = (b.priority < m_lowestPriority)  ? b.priority : m_lowestPriority;
}

void BuildOrderQueue::queueAsHighestPriority(const MetaType & type, bool blocking)
{
    // the new priority will be higher
    int newPriority = m_highestPriority + m_defaultPrioritySpacing;

    // queue the item
    queueItem(BuildOrderItem(type, newPriority, blocking));
}

void BuildOrderQueue::queueAsLowestPriority(const MetaType & type, bool blocking)
{
    // the new priority will be higher
    int newPriority = m_lowestPriority - m_defaultPrioritySpacing;

    // queue the item
    queueItem(BuildOrderItem(type, newPriority, blocking));
}

void BuildOrderQueue::removeHighestPriorityItem()
{
    // remove the back element of the vector
    m_queue.pop_back();

    // if the list is not empty, set the highest accordingly
    m_highestPriority = m_queue.empty() ? 0 : m_queue.back().priority;
    m_lowestPriority  = m_queue.empty() ? 0 : m_lowestPriority;
}

void BuildOrderQueue::removeCurrentHighestPriorityItem()
{
    // remove the back element of the vector
    m_queue.erase(m_queue.begin() + m_queue.size() - 1 - m_numSkippedItems);

    //assert((int)(queue.size()) < size);

    // if the list is not empty, set the highest accordingly
    m_highestPriority = m_queue.empty() ? 0 : m_queue.back().priority;
    m_lowestPriority  = m_queue.empty() ? 0 : m_lowestPriority;
}

size_t BuildOrderQueue::size()
{
    return m_queue.size();
}

bool BuildOrderQueue::isEmpty()
{
    return (m_queue.size() == 0);
}

BuildOrderItem BuildOrderQueue::operator [] (int i)
{
    return m_queue[i];
}

std::string BuildOrderQueue::getQueueInformation() const
{
    size_t reps = m_queue.size() < 30 ? m_queue.size() : 30;
    std::stringstream ss;

    // for each unit in the queue
    for (size_t i(0); i<reps; i++)
    {
		auto item = m_queue[m_queue.size() - 1 - i];
        const MetaType & type = item.type;
		ss << type.getName() << std::setw(30 - item.type.getName().length()) << std::right << " [" << item.priority << "]";
		if (item.blocking)
		{
			ss << " (B)";
		}
		ss << "\n";
    }

    return ss.str();
}

bool BuildOrderQueue::contains(const MetaType & type)
{
	auto it = m_queue.begin();
	while (it != m_queue.end())
	{
		if(type.getUnitType().getAPIUnitType() == (*it++).type.getUnitType().getAPIUnitType())
			return true;
	}
	return false;
}


BuildOrderItem::BuildOrderItem(const MetaType & t, int p, bool b)
    : type(t)
    , priority(p)
    , blocking(b)
{
}

bool BuildOrderItem::operator < (const BuildOrderItem & x) const
{
	return priority < x.priority;
}

bool BuildOrderItem::operator == (const BuildOrderItem & x) const
{
	return type.getUnitType().getAPIUnitType() == x.type.getUnitType().getAPIUnitType() && priority == x.priority;
}