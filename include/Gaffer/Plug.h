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

#ifndef GAFFER_PLUG_H
#define GAFFER_PLUG_H

#include "Gaffer/GraphComponent.h"

#include "IECore/Object.h"

namespace Gaffer
{

IE_CORE_FORWARDDECLARE( Plug )
IE_CORE_FORWARDDECLARE( Node )

/// let's not have DoublePlug or DoubleVectorPlug
///		- keep all real types as float ?? really?? why not use double if we use only one type?
///		- and all integers as int? - except for maybe UIntVectorData?
///
/// Cannot dirty an input plug without a connection
///		- and input plugs revert to default (or current value) on disconnect?
/// Expressions
///		- Node? or intrinsic to Plug somehow? do they dirty automatically? or compute each time?
///		- it'd be nice not to have to call IntData::value() just to get values from Plugs in these
/// Memory management
///		- Is this where we flush less often used values into a disk cache? and load 'em again when needed?
///
/// Image plugs with partial computation - how would they work????
///
class Plug : public GraphComponent
{

	public :
	
		enum Direction
		{
			Invalid = 0,
			In = 1,
			Out = 2
		};
	
		enum Flags
		{
			None = 0x00000000,
			/// Dynamic plugs are those which are created outside of the constructor
			/// for a Node. This means that their value alone is not enough when serialising
			/// a script - instead the full Plug definition is serialised so it can
			/// be recreated fully upon loading.
			Dynamic = 0x00000001,
			All = Dynamic
		};
	
		Plug( const std::string &name=staticTypeName(), Direction direction=In, unsigned flags=None );
		virtual ~Plug();

		IE_CORE_DECLARERUNTIMETYPEDEXTENSION( Plug, PlugTypeId, GraphComponent );

		/// @name Parent-child relationships
		//////////////////////////////////////////////////////////////////////
		//@{
		/// Accepts no children.
		virtual bool acceptsChild( ConstGraphComponentPtr potentialChild ) const;
		/// Accepts only Nodes or Plugs as a parent.
		virtual bool acceptsParent( const GraphComponent *potentialParent ) const;
		/// Just returns ancestor<Node>() as a syntactic convenience.
		Node *node();
		/// Just returns ancestor<Node>() as a syntactic convenience.
		const Node *node() const;
		//@}

		Direction direction() const;
		
		/// Returns the current state of the flags.
		unsigned getFlags() const;
		/// Returns true if all the flags passed are currently set.
		bool getFlags( unsigned flags ) const;
		/// Sets the current state of the flags.
		void setFlags( unsigned flags );
		/// Sets or unsets the specified flags depending on the enable
		/// parameter. All other flags remain at their current values.
		void setFlags( unsigned flags, bool enable );
		
		/// @name Connections
		///////////////////////////////////////////////////////////////////////
		//@{
		typedef std::list<Plug *> OutputContainer;
		/// Accepts any input, provided that direction()==In.
		virtual bool acceptsInput( ConstPlugPtr input ) const;
		/// Pass input as 0 to remove any existing inputs.
		/// \undoable
		virtual void setInput( PlugPtr input );
		template<typename T>
		typename T::Ptr getInput();
		template<typename T>
		typename T::ConstPtr getInput() const;
		void removeOutputs();
		const OutputContainer &outputs() const;
		//@}
					
	private :

		void setInputInternal( PlugPtr input, bool emit );

		Direction m_direction;
		Plug *m_input;
		OutputContainer m_outputs;
		unsigned m_flags;
				
};

IE_CORE_DECLAREPTR( Plug );

} // namespace Gaffer

#include "Gaffer/Plug.inl"

#endif // GAFFER_PLUG_H
