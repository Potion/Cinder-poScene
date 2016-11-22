#include "TextComponent.h"

#include "cinder/app/App.h"

TextComponentRef TextComponent::create()
{
	TextComponentRef ref = std::shared_ptr<TextComponent>(new TextComponent());
	ref->setup();
	return ref;
}

TextComponent::TextComponent() 
{
}

TextComponent::~TextComponent() 
{
}

void TextComponent::setup()
{
	//	Create the ci textbox
	mCiTextBox = ci::TextBox();
	mCiTextBox.size(ci::app::getWindowWidth() - 100, ci::app::getWindowHeight() * 2);
	mCiTextBox.color(ci::Color(1, 1, 1));
	mCiTextBox.text(generateText());
	mCiTextBox.font(ci::Font("Georgia", 14));
	
	//	Add the ci textbox to the text node
	mTextBox = TextView::create(mCiTextBox);
	addChild(mTextBox);
}

std::string TextComponent::generateText()
{
	std::string text;
	
	text = "Ignore the squirrels, you'll never catch them anyway stick butt in face, and stare at ceiling spit up on light gray carpet instead of adjacent linoleum but meow or intrigued by the shower. Peer out window, chatter at birds, lure them to mouth poop in litter box, scratch the walls, but plan steps for world domination. Who's the baby throwup on your pillow, yet chew iPad power cord, yet get video posted to internet for chasing red dot but pelt around the house and up and down stairs chasing phantoms give attitude. Loves cheeseburgers who's the baby. \n\nHave secret plans intrigued by the shower hack up furballs, for kick up litter so loves cheeseburgers, for poop on grasses scamper. Leave hair everywhere stand in front of the computer screen, or leave fur on owners clothes hide at bottom of staircase to trip human. Leave dead animals as gifts. Purr for no reason chew foot, so curl into a furry donut meowing non stop for food chew foot rub face on everything cat snacks. Favor packaging over toy stretch, yet shove bum in owner's face like camera lens i am the best. Lick butt meowwww get video posted to internet for chasing red dot stare at the wall, play with food and get confused by dust give attitude my left donut is missing, as is my right. Eat and than sleep on your face destroy couch, and scratch leg; meow for can opener to feed me. \n\nPaw at your fat belly shove bum in owner's face like camera lens or burrow under covers, and pelt around the house and up and down stairs chasing phantoms and put toy mouse in food bowl run out of litter box at full speed i like big cats and i can not lie but cough furball. Stick butt in face sweet beast sleep on dog bed, force dog to sleep on floor yet all of a sudden cat goes crazy. Play riveting piece on synthesizer keyboard claws in your leg hide from vacuum cleaner jump around on couch, meow constantly until given food, but claws in your leg for hack up furballs, stare at ceiling. \n\nInspect anything brought into the house. Mark territory scream at teh bath and attack feet. Leave hair everywhere why must they do that, yet shove bum in owner's face like camera lens. Stick butt in face cat slap dog in face jump launch to pounce upon little yarn mouse, bare fangs at toy run hide in litter box until treats are fed.";
	
	text += "\n\nPlay riveting piece on synthesizer keyboard chew iPad power cord pelt around the house and up and down stairs chasing phantoms. Meowing non stop for food hide at bottom of staircase to trip human so i am the best. Sleep nap pooping rainbow while flying in a toasted bread costume in space leave dead animals as gifts. Intently sniff hand jump launch to pounce upon little yarn mouse, bare fangs at toy run hide in litter box until treats are fed. Lick butt poop on grasses. Hack up furballs favor packaging over toy but always hungry intently sniff hand. Make meme, make cute face mark territory get video posted to internet for chasing red dot pooping rainbow while flying in a toasted bread costume in space for scratch leg; meow for can opener to feed me hate dog. \n\nIntently sniff hand stretch ears back wide eyed yet spit up on light gray carpet instead of adjacent linoleum bathe private parts with tongue then lick owner's face peer out window, chatter at birds, lure them to mouth. Loves cheeseburgers. Bathe private parts with tongue then lick owner's face behind the couch. Hack up furballs lick arm hair under the bed hunt by meowing loudly at 5am next to human slave food dispenser and curl into a furry donut so eat grass, throw it back up but eat and than sleep on your face. Poop in litter box, scratch the walls. Under the bed spread kitty litter all over house get video posted to internet for chasing red dot caticus cuteicus under the bed chew foot, and behind the couch. Stare at ceiling. Refuse to drink water except out of someone's glass. \n\nChase mice get video posted to internet for chasing red dot caticus cuteicus sun bathe, or see owner, run in terror i am the best. Get video posted to internet for chasing red dot. Leave dead animals as gifts then cats take over the world so purr while eating, yet claws in your leg but chase mice. Destroy couch. Lounge in doorway my left donut is missing, as is my right behind the couch, and sleep on dog bed, force dog to sleep on floor.";
	
	return text;
}