/*
 * Copyright (c) 1991 Stanford University
 * Copyright (c) 1991 Silicon Graphics, Inc.
 *
 * Permission to use, copy, modify, distribute, and sell this software and 
 * its documentation for any purpose is hereby granted without fee, provided
 * that (i) the above copyright notices and this permission notice appear in
 * all copies of the software and related documentation, and (ii) the names of
 * Stanford and Silicon Graphics may not be used in any advertising or
 * publicity relating to the software without the specific, prior written
 * permission of Stanford and Silicon Graphics.
 * 
 * THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND, 
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY 
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.  
 *
 * IN NO EVENT SHALL STANFORD OR SILICON GRAPHICS BE LIABLE FOR
 * ANY SPECIAL, INCIDENTAL, INDIRECT OR CONSEQUENTIAL DAMAGES OF ANY KIND,
 * OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS,
 * WHETHER OR NOT ADVISED OF THE POSSIBILITY OF DAMAGE, AND ON ANY THEORY OF 
 * LIABILITY, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE 
 * OF THIS SOFTWARE.
 */

/*
 * Adjuster - control an adjustable
 */

#include <IV-look/adjuster.h>
#include <IV-look/adjustable.h>

Adjuster::Adjuster(Adjustable* a) {
    adjustable_ = a;
    if (a != nil) {
	a->attach(this);
    }
}

Adjuster::~Adjuster() {
    if (adjustable_ != nil) {
	adjustable_->detach(this);
    }
}

Adjustable* Adjuster::subject() const { return adjustable_; }

void Adjuster::update(DimensionName) { }
void Adjuster::update_all() { }

void Adjuster::connect(Adjustable* a) {
    if (adjustable_ != nil) {
	adjustable_->detach(this);
    }
    adjustable_ = a;
    if (a != nil) {
	a->attach(this);
    }
}

void Adjuster::disconnect(Adjustable*) {
    adjustable_ = nil;
}
