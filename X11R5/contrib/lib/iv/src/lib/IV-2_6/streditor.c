/*
 * Copyright (c) 1987, 1988, 1989, 1990, 1991 Stanford University
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
 * StringEditor - interactive editor for character strings
 */

#include <IV-look/2.6/InterViews/button.h>
#include <IV-look/2.6/InterViews/streditor.h>
#include <InterViews/bitmap.h>
#include <InterViews/cursor.h>
#include <InterViews/font.h>
#include <InterViews/sensor.h>
#include <InterViews/world.h>
#include <InterViews/2.6/InterViews/painter.h>
#include <InterViews/2.6/InterViews/shape.h>
#include <InterViews/2.6/InterViews/textbuffer.h>
#include <InterViews/2.6/InterViews/textdisplay.h>
#include <InterViews/Bitmaps/hand.bm>
#include <InterViews/Bitmaps/handMask.bm>
#include <InterViews/Bitmaps/lfast.bm>
#include <InterViews/Bitmaps/lfastMask.bm>
#include <InterViews/Bitmaps/rfast.bm>
#include <InterViews/Bitmaps/rfastMask.bm>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

static const int BUFFERSIZE = 1000;

StringEditor::StringEditor (
    ButtonState* s, const char* samp, const char* done
) {
    Init(s, samp, done);
}

StringEditor::StringEditor (
    const char* name, ButtonState* s, const char* samp, const char* done
) {
    SetInstance(name);
    Init(s, samp, done);
}

void StringEditor::Init (ButtonState* s, const char* samp, const char* d) {
    SetClassName("StringEditor");
    sample = strcpy(new char[strlen(samp) + 1], samp);
    size = BUFFERSIZE;
    buffer = new char[size];
    text = new TextBuffer(buffer, 0, size);
    left = 0;
    right = 0;
    subject = s;
    if (subject != nil) {
        subject->Attach(this);
    }
    done = strcpy(new char[strlen(d) + 1], d);
    display = new TextDisplay();
    display->CaretStyle(NoCaret);
    input = new Sensor();
    input->Catch(KeyEvent);
    input->Catch(DownEvent);
    Message(sample);
}

StringEditor::~StringEditor () {
    if (subject != nil) {
        subject->Detach(this);
    }
    delete text;
    delete sample;
    delete done;
    delete buffer;
    delete display;
}

static Cursor* handCursor;
static Cursor* leftCursor;
static Cursor* rightCursor;

void StringEditor::Reconfig () {
    if (handCursor == nil) {
        Bitmap hand(
            hand_bits, hand_width, hand_height, hand_x_hot, hand_y_hot
        );
        Bitmap handmask(hand_mask_bits, hand_mask_width, hand_mask_height);
        Bitmap left(
            lfast_bits, lfast_width, lfast_height, lfast_x_hot, lfast_y_hot
        );
        Bitmap leftmask(lfast_mask_bits, lfast_mask_width, lfast_mask_height);
        Bitmap right(
            rfast_bits, rfast_width, rfast_height, rfast_x_hot, rfast_y_hot
        );
        Bitmap rightmask(rfast_mask_bits, rfast_mask_width, rfast_mask_height);

        handCursor = new Cursor(
            &hand, &handmask, output->GetFgColor(), output->GetBgColor()
        );
        leftCursor = new Cursor(
            &left, &leftmask, output->GetFgColor(), output->GetBgColor()
        );
        rightCursor = new Cursor(
            &right, &rightmask, output->GetFgColor(), output->GetBgColor()
        );
    }

    const Font* f = output->GetFont();
    shape->Rect(f->Width(sample), f->Height());
    shape->Rigid(hfil, hfil, 0, 0);
    display->LineHeight(f->Height());
}

void StringEditor::Resize () {
    display->Draw(output, canvas);
    display->Resize(0, 0, xmax, ymax);
    Select(left, right);
}

void StringEditor::Redraw (IntCoord l, IntCoord b, IntCoord r, IntCoord t) {
    display->Draw(output, canvas);
    display->Redraw(l, b, r, t);
}

void StringEditor::Message (const char* t) {
    text->Delete(text->BeginningOfText(), text->Length());
    text->Insert(0, t, strlen(t));
    int bol = text->BeginningOfLine(0);
    int eol = text->EndOfLine(0);
    display->Draw(output, canvas);
    display->ReplaceText(0, text->Text(bol, eol), eol - bol);
    Select(eol);
}

void StringEditor::Select (int l) {
    Select(l, l);
}

void StringEditor::Select (int l, int r) {
    display->Draw(output, canvas);
    int origin = display->Left(0, 0);
    if (display->Left(0, r - 1) < 0) {
        origin += xmax/2 - display->Left(0, r - 1);
    } else if (display->Right(0, r + 1) > xmax) {
        origin += xmax/2 - display->Right(0, r + 1);
    }
    origin = Math::min(
	0, Math::max(Math::min(0, xmax - display->Width()), origin)
    );
    display->Scroll(0, origin, ymax);
    DoSelect(l, r);
}

void StringEditor::DoSelect (int l, int r) {
    display->Draw(output, canvas);
    if (l > r) {
        int tmp = r;
        r = l;
        l = tmp;
    }
    l = Math::max(l, text->BeginningOfLine(left));
    r = Math::min(r, text->EndOfLine(right));
    if (r < left || l > right) {
        if (right > left) {
            display->Style(0, left, 0, right-1, Plain);
        }
        if (r > l) {
            display->Style(0, l, 0, r-1, Reversed);
        }
    } else {
        if (l < left) {
            display->Style(0, l, 0, left-1, Reversed);
        } else if (l > left) {
            display->Style(0, left, 0, l-1, Plain);
        }
        if (r > right) {
            display->Style(0, right, 0, r-1, Reversed);
        } else if (r < right) {
            display->Style(0, r, 0, right-1, Plain);
        }
    }
    left = l;
    right = r;
    if (left == right) {
        display->Caret(0, left);
    } else {
        display->Caret(-1, 0);
    }
}

void StringEditor::Edit () {
    Event e;
    e.target = nil;
    e.eventType = EnterEvent;
    Handle(e);
}

void StringEditor::Edit (const char* t, int l, int r) {
    Message(t);
    Select(l, r);
    Edit();
}

const char* StringEditor::Text () {
    return text->Text();
}

boolean StringEditor::HandleChar (char c) {
    if (strchr(done, c) != nil) {
        if (subject != nil) {
            subject->SetValue(c);
        }
        return true;
    } else {
        switch (c) {
          case SEBeginningOfLine:
            Select(text->BeginningOfLine(left));
            break;
          case SEEndOfLine:
            Select(text->EndOfLine(right));
            break;
          case SESelectAll:
            Select(text->BeginningOfText(), text->EndOfText());
            break;
          case SESelectWord:
            Select(
                text->BeginningOfWord(text->PreviousCharacter(left)), right
            );
            break;
          case SEPreviousCharacter:
            Select(text->PreviousCharacter(left));
            break;
          case SENextCharacter:
            Select(text->NextCharacter(right));
            break;
          case SEDeleteNextCharacter:
            if (left == right) {
                right = text->NextCharacter(right);
            }
            InsertText("", 0);
            break;
          case SEDeletePreviousCharacter:
          case SEDeletePreviousCharacterAlt:
            if (left == right) {
                left = text->PreviousCharacter(left);
            }
            InsertText("", 0);
            break;
          default:
            if (!iscntrl(c)) {
                InsertText(&c, 1);
            }
            break;
        }
        return false;
    }
}

void StringEditor::InsertText (const char* t, int len) {
    display->Draw(output, canvas);
    if (left != right) {
        text->Delete(left, right-left);
        display->DeleteText(0, left, right-left);
    }
    text->Insert(left, t, len);
    display->InsertText(0, left, t, len);
    int l = left;
    while (len > 0) {
        l = text->NextCharacter(l);
        --len;
    }
    Select(l);
}

void StringEditor::Handle (Event& e) {
    boolean done = false;
    World* world = GetWorld();
    display->Draw(output, canvas);
    display->CaretStyle(BarCaret);
    do {
        switch (e.eventType) {
          case KeyEvent:
            if (e.len != 0) {
                done = HandleChar(e.keystring[0]);
            }
            break;
          case DownEvent:
            if (e.target == this) {
                int origin = display->Left(0, 0);
                int width = display->Width();
                if (e.button == LEFTMOUSE) {
                    int start = display->LineIndex(0, e.x);
                    do {
                        if (e.x < 0) {
                            origin = Math::min(0, origin - e.x);
                        } else if (e.x > xmax) {
                            origin = Math::max(
				xmax - width, origin - (e.x - xmax)
			    );
                        }
                        display->Scroll(0, origin, ymax);
                        DoSelect(start, display->LineIndex(0, e.x));
                        Poll(e);
                    } while (e.leftmouse);
                } else if (e.button == MIDDLEMOUSE) {
                    Cursor* origCursor = GetCursor();
                    SetCursor(handCursor);
                    int x = e.x;
                    do {
                        origin += e.x - x;
                        origin = Math::min(
			    0, Math::max(Math::min(0, xmax - width), origin)
			);
                        display->Scroll(0, origin, ymax);
                        x = e.x;
                        Poll(e);
                    } while (e.middlemouse);
                    SetCursor(origCursor);
                } else if (e.button == RIGHTMOUSE) {
                    Cursor* origCursor = GetCursor();
                    int x = e.x;
                    do {
                        origin += x - e.x;
                        origin = Math::min(
			    0, Math::max(Math::min(0, xmax - width), origin)
			);
                        display->Scroll(0, origin, ymax);
                        if (e.x - x < 0) {
                            SetCursor(leftCursor);
                        } else {
                            SetCursor(rightCursor);
                        }
                        Poll(e);
                    } while (e.rightmouse);
                    SetCursor(origCursor);
                }
            } else {
                UnRead(e);
                done = true;
            }
            break;
        }
        if (!done) {
            Read(e);
        }
    } while (!done && !world->done());
    display->CaretStyle(NoCaret);
}
