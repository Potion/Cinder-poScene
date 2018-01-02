#pragma once

/*	Created by ___FULLUSERNAME___ on ___DATE___.
 *	Copyright ___YEAR___ ___ORGANIZATIONNAME___. All rights reserved.
 */

#include "poScene/ViewController.h"

namespace po { 
	namespace scene {

		class ___FILEBASENAME___;
		typedef std::shared_ptr< ___FILEBASENAME___ > ___FILEBASENAME___Ref;

		class ___FILEBASENAME___ : public po::scene::ViewController
		{
		public:
			static ___FILEBASENAME___Ref create();
			void viewDidLoad() override;

		protected:
			 ___FILEBASENAME___();
		 
		};

	}
} //  Namespace: po::scene