#!/bin/bash

# helper script for xcode samples
# uses xcpretty for formatting output https://github.com/supermarin/xcpretty
# install with `gem install xcpretty`

echo "Ready to build all poScene examples"
read -p "Press enter to continue"
echo "Building all poScene examples..."

# Execute each build in a subshell
# ref: http://stackoverflow.com/questions/786376/how-do-i-run-a-program-with-a-different-working-directory-from-current-from-lin

(cd AlignmentSample/xcode && exec xcodebuild | xcpretty)
(cd AnimationSample/xcode && exec xcodebuild | xcpretty)
(cd BoundsSample/xcode && exec xcodebuild | xcpretty)
(cd DragAndDrop/xcode && exec xcodebuild | xcpretty)
(cd HierarchySample/xcode && exec xcodebuild | xcpretty)
(cd MaskingSample/xcode && exec xcodebuild | xcpretty)
(cd MouseEventsSample/xcode && exec xcodebuild | xcpretty)
(cd ShapeTextureSample/xcode && exec xcodebuild | xcpretty)
(cd TextSample/xcode && exec xcodebuild | xcpretty)
(cd TextSampleAdvanced/xcode && exec xcodebuild | xcpretty)
(cd VideoSample/xcode && exec xcodebuild | xcpretty)
(cd VideoSampleAdvanced/xcode && exec xcodebuild | xcpretty)

echo "...Finished building poScene examples."
