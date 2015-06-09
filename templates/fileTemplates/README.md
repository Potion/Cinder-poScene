#File templates

## Xcode:

Xcode Template files live under your user library folder, typically the would be found under the path below:
```
~/Library/Developer/Xcode/Templates/
```

Create a folder named ```Templates``` in ```~/Library/Developer/Xcode```, if it doesn't exist.

### Option 1 - Create a symbolic link:

```bash
$ sudo ln -s <CINDER PATH>/blocks/Cinder-poScene/templates/fileTemplates/Xcode/poScene_subclass.xctemplate ~/Library/Developer/Xcode/Templates/poScene_subclass.xctemplate 
```

### Option 2 - Copy the files manually

Copy ```poScene_subclass.xctemplate```, included in ```templates/fileTemplates/Xcode```, to ```~/Library/Developer/Xcode/Templates```

## VS2013:
Unfortunately there are no file templates available for c++ under Visual Studio. use find/replace.  