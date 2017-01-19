#!/bin/bash

echo "Ready to build all poScene examples"
read -p "Press enter to continue"
echo "Building all poScene examples..."

# Execute each build in a subshell
# ref: http://stackoverflow.com/questions/786376/how-do-i-run-a-program-with-a-different-working-directory-from-current-from-lin

(cd AlignmentSample/xcode && exec xcodebuild)
(cd AnimationSample/xcode && exec xcodebuild)
(cd BoundsSample/xcode && exec xcodebuild)
(cd DragAndDrop/xcode && exec xcodebuild)
(cd HierarchySample/xcode && exec xcodebuild)
(cd MaskingSample/xcode && exec xcodebuild)
(cd MouseEventsSample/xcode && exec xcodebuild)
(cd ShapeTextureSample/xcode && exec xcodebuild)
(cd TextSample/xcode && exec xcodebuild)
(cd TextSampleAdvanced/xcode && exec xcodebuild)
(cd VideoSample/xcode && exec xcodebuild)
(cd VideoSampleAdvanced/xcode && exec xcodebuild)

echo "...Finished building poScene examples."
