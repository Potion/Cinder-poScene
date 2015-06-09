## Setup templates for Xcode

Create a folder named ```Templates``` in ```~/Library/Developer/Xcode```, if it doesn't exist.

### Create a symbolic link:

```bash
$ sudo ln -s <CINDER PATH>/blocks/Cinder-poScene/templates/fileTemplates/Xcode/poScene_subclass.xctemplate ~/Library/Developer/Xcode/Templates/poScene_subclass.xctemplate 
```

### Copy the files manually

Copy ```poScene_subclass.xctemplate```, included in ```templates/fileTemplates/Xcode```, to ```~/Library/Developer/Xcode/Templates```