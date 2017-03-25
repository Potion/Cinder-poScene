#pragma once

#include <map>

namespace po
{
	namespace scene
	{
		typedef std::shared_ptr<class ViewEventControllerBase> ViewEventControllerBaseRef;
		class ViewEventControllerBase
		{
			public:
				virtual void test() = 0;
		};

		template<typename EventT, typename EventTypeT, typename SignalTypeT>
		class ViewEventController : public ViewEventControllerBase
		{
			public:
				void test() override {};

				SignalTypeT& getSignal( EventTypeT type )
				{
					return mEventSignals[type];
				}

				void emitEvent( EventT& event )
				{
					mEventSignals[event.getType()].emit( event );
				}

				bool isEligibleForInteractionEvent( const EventTypeT& type )
				{
					if( ( mEventSignals.count( type ) && mEventSignals[type].getNumSlots() != 0 ) ) {
						return true;
					}

					return false;
				}

			private:
				std::map<EventTypeT, SignalTypeT> mEventSignals;
		};

	}
}