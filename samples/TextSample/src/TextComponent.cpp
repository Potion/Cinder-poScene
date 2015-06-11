#include "TextComponent.h"

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
}