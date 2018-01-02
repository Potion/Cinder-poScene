#pragma once

/*	Created by ___FULLUSERNAME___ on ___DATE___.
 *	Copyright ___YEAR___ ___ORGANIZATIONNAME___. All rights reserved.
 */

namespace po { 
	namespace scene {

		class ___FILEBASENAME___;
		typedef std::shared_ptr< ___FILEBASENAME___ > ___FILEBASENAME___Ref;

		class ___FILEBASENAME___ : public po::scene::View
		{
		public:
			static ___FILEBASENAME___Ref create();
			virtual void setup();

		protected:
			___FILEBASENAME___();
			virtual void update();
		    virtual void draw();
		    virtual ~___FILEBASENAME___();
	
		};

	}
} //  Namespace: po::scene