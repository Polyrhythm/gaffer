//////////////////////////////////////////////////////////////////////////
//  
//  Copyright (c) 2011, John Haddon. All rights reserved.
//  
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions are
//  met:
//  
//      * Redistributions of source code must retain the above
//        copyright notice, this list of conditions and the following
//        disclaimer.
//  
//      * Redistributions in binary form must reproduce the above
//        copyright notice, this list of conditions and the following
//        disclaimer in the documentation and/or other materials provided with
//        the distribution.
//  
//      * Neither the name of John Haddon nor the names of
//        any other contributors to this software may be used to endorse or
//        promote products derived from this software without specific prior
//        written permission.
//  
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS
//  IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
//  THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
//  PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
//  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
//  EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
//  PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
//  PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
//  LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
//  NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
//  SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//  
//////////////////////////////////////////////////////////////////////////

#include "GafferUI/TextGadget.h"
#include "GafferUI/Style.h"

#include "IECore/SimpleTypedData.h"

using namespace GafferUI;
using namespace IECore;
using namespace boost;

IE_CORE_DEFINERUNTIMETYPED( TextGadget );

TextGadget::TextGadget( const std::string &text )
	:	Gadget( staticTypeName() ), m_font( 0 ), m_text( text )
{
}

TextGadget::~TextGadget()
{
}

IECore::ConstFontPtr TextGadget::getFont() const
{
	return IECore::constPointerCast<Font>( m_font ? m_font : getStyle()->labelFont() );
}

void TextGadget::setFont( IECore::FontPtr font )
{
	m_font = font;
}

const std::string &TextGadget::getText() const
{
	return m_text;
}

void TextGadget::setText( const std::string &text )
{
	if( text!=m_text )
	{
		m_text = text;
		renderRequestSignal()( this );
	}
}

Imath::Box3f TextGadget::bound() const
{
	Imath::Box2f b = getFont()->bound( m_text );
	return Imath::Box3f( Imath::V3f( b.min.x, b.min.y, 0 ), Imath::V3f( b.max.x, b.max.y, 0 ) );
}

void TextGadget::doRender( IECore::RendererPtr renderer ) const
{
	renderer->attributeBegin();
		/// \todo Have the color and wotnot come from the style.
		renderer->setAttribute( "color", new IECore::Color3fData( Imath::Color3f( 0, 0, 0 ) ) );
		renderer->setAttribute( "gl:textPrimitive:type", new IECore::StringData( "sprite" ) );
		renderer->text( getFont()->fileName(), m_text );
	renderer->attributeEnd();
}
