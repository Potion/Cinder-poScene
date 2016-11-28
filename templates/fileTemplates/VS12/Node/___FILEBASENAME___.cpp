#include "___FILEBASENAME___.h"

namespace po 
{ 
	namespace scene {

		___FILEBASENAME___Ref ___FILEBASENAME___::create() 
		{
	        ___FILEBASENAME___Ref m_ref = std::shared_ptr<___FILEBASENAME___>(new ___FILEBASENAME___());
			m_ref.setup();
			return m_ref;
	    }

		___FILEBASENAME___::___FILEBASENAME___() 
		{
		}

		___FILEBASENAME___::~___FILEBASENAME___() 
		{
		}

		void ___FILEBASENAME___::setup()
		{
		}
		
		void ___FILEBASENAME___::update()
		{
		}

		void ___FILEBASENAME___::draw()
		{
		}

	} 
} //  Namespace: po::scene
