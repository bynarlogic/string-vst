/*******************************************************************************************************************
Copyright (c) 2023 Cycling '74

The code that Max generates automatically and that end users are capable of
exporting and using, and any associated documentation files (the “Software”)
is a work of authorship for which Cycling '74 is the author and owner for
copyright purposes.

This Software is dual-licensed either under the terms of the Cycling '74
License for Max-Generated Code for Export, or alternatively under the terms
of the General Public License (GPL) Version 3. You may use the Software
according to either of these licenses as it is most appropriate for your
project on a case-by-case basis (proprietary or not).

A) Cycling '74 License for Max-Generated Code for Export

A license is hereby granted, free of charge, to any person obtaining a copy
of the Software (“Licensee”) to use, copy, modify, merge, publish, and
distribute copies of the Software, and to permit persons to whom the Software
is furnished to do so, subject to the following conditions:

The Software is licensed to Licensee for all uses that do not include the sale,
sublicensing, or commercial distribution of software that incorporates this
source code. This means that the Licensee is free to use this software for
educational, research, and prototyping purposes, to create musical or other
creative works with software that incorporates this source code, or any other
use that does not constitute selling software that makes use of this source
code. Commercial distribution also includes the packaging of free software with
other paid software, hardware, or software-provided commercial services.

For entities with UNDER $200k in annual revenue or funding, a license is hereby
granted, free of charge, for the sale, sublicensing, or commercial distribution
of software that incorporates this source code, for as long as the entity's
annual revenue remains below $200k annual revenue or funding.

For entities with OVER $200k in annual revenue or funding interested in the
sale, sublicensing, or commercial distribution of software that incorporates
this source code, please send inquiries to licensing@cycling74.com.

The above copyright notice and this license shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

Please see
https://support.cycling74.com/hc/en-us/articles/10730637742483-RNBO-Export-Licensing-FAQ
for additional information

B) General Public License Version 3 (GPLv3)
Details of the GPLv3 license can be found at: https://www.gnu.org/licenses/gpl-3.0.html
*******************************************************************************************************************/

#include "RNBO_Common.h"
#include "RNBO_AudioSignal.h"

namespace RNBO {


#define trunc(x) ((Int)(x))

#if defined(__GNUC__) || defined(__clang__)
    #define RNBO_RESTRICT __restrict__
#elif defined(_MSC_VER)
    #define RNBO_RESTRICT __restrict
#endif

#define FIXEDSIZEARRAYINIT(...) { }

class rnbomatic : public PatcherInterfaceImpl {
public:

class RNBOSubpatcher_126 : public PatcherInterfaceImpl {
    
    friend class rnbomatic;
    
    public:
    
    RNBOSubpatcher_126()
    {
    }
    
    ~RNBOSubpatcher_126()
    {
    }
    
    virtual rnbomatic* getPatcher() const {
        return static_cast<rnbomatic *>(_parentPatcher);
    }
    
    rnbomatic* getTopLevelPatcher() {
        return this->getPatcher()->getTopLevelPatcher();
    }
    
    void cancelClockEvents()
    {
        getEngine()->flushClockEvents(this, 861222235, false);
        getEngine()->flushClockEvents(this, -1494586265, false);
    }
    
    inline number linearinterp(number frac, number x, number y) {
        return x + (y - x) * frac;
    }
    
    SampleIndex currentsampletime() {
        return this->audioProcessSampleCount + this->sampleOffsetIntoNextAudioBuffer;
    }
    
    number mstosamps(MillisecondTime ms) {
        return ms * this->sr * 0.001;
    }
    
    inline number cubicinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = z - y - w + x;
        number f1 = w - x - f0;
        number f2 = y - w;
        number f3 = x;
        return f0 * a * a2 + f1 * a2 + f2 * a + f3;
    }
    
    inline number splineinterp(number a, number w, number x, number y, number z) {
        number a2 = a * a;
        number f0 = -0.5 * w + 1.5 * x - 1.5 * y + 0.5 * z;
        number f1 = w - 2.5 * x + 2 * y - 0.5 * z;
        number f2 = -0.5 * w + 0.5 * y;
        return f0 * a * a2 + f1 * a2 + f2 * a + x;
    }
    
    inline number cosT8(number r) {
        number t84 = 56.0;
        number t83 = 1680.0;
        number t82 = 20160.0;
        number t81 = 2.4801587302e-05;
        number t73 = 42.0;
        number t72 = 840.0;
        number t71 = 1.9841269841e-04;
    
        if (r < 0.785398163397448309615660845819875721 && r > -0.785398163397448309615660845819875721) {
            number rr = r * r;
            return 1.0 - rr * t81 * (t82 - rr * (t83 - rr * (t84 - rr)));
        } else if (r > 0.0) {
            r -= 1.57079632679489661923132169163975144;
            number rr = r * r;
            return -r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        } else {
            r += 1.57079632679489661923132169163975144;
            number rr = r * r;
            return r * (1.0 - t71 * rr * (t72 - rr * (t73 - rr)));
        }
    }
    
    inline number cosineinterp(number frac, number x, number y) {
        number a2 = (1.0 - this->cosT8(frac * 3.14159265358979323846)) / (number)2.0;
        return x * (1.0 - a2) + y * a2;
    }
    
    number samplerate() {
        return this->sr;
    }
    
    Index vectorsize() {
        return this->vs;
    }
    
    number maximum(number x, number y) {
        return (x < y ? y : x);
    }
    
    Index getNumMidiInputPorts() const {
        return 1;
    }
    
    void processMidiEvent(MillisecondTime time, int port, ConstByteArray data, Index length) {
        this->updateTime(time);
        this->notein_01_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
    }
    
    Index getNumMidiOutputPorts() const {
        return 0;
    }
    
    void process(
        const SampleValue * const* inputs,
        Index numInputs,
        SampleValue * const* outputs,
        Index numOutputs,
        Index n
    ) {
        RNBO_UNUSED(numInputs);
        RNBO_UNUSED(inputs);
        this->vs = n;
        this->updateTime(this->getEngine()->getCurrentTime());
        SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
        SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    
        if (this->getIsMuted())
            return;
    
        this->curve_01_perform(this->signals[0], n);
    
        this->lores_01_perform(
            this->lores_01_cutoff,
            this->lores_01_resonance,
            this->signals[0],
            this->signals[1],
            n
        );
    
        this->gen_01_perform(
            this->signals[1],
            this->gen_01_feedback,
            this->gen_01_note,
            this->gen_01_dampen,
            this->gen_01_a,
            this->gen_01_b,
            this->signals[2],
            this->signals[3],
            n
        );
    
        this->dspexpr_01_perform(
            this->signals[2],
            this->dspexpr_01_in2,
            this->dspexpr_01_in3,
            this->signals[1],
            n
        );
    
        this->signaladder_01_perform(this->signals[1], out1, out1, n);
        this->signaladder_02_perform(this->signals[1], out2, out2, n);
        this->numbertilde_01_perform(this->signals[3], this->dummyBuffer, n);
        this->scopetilde_01_perform(this->signals[0], this->zeroBuffer, n);
        this->stackprotect_perform(n);
        this->audioProcessSampleCount += this->vs;
    }
    
    void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
        if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
            Index i;
    
            for (i = 0; i < 4; i++) {
                this->signals[i] = resizeSignal(this->signals[i], this->maxvs, maxBlockSize);
            }
    
            this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
            this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
            this->didAllocateSignals = true;
        }
    
        const bool sampleRateChanged = sampleRate != this->sr;
        const bool maxvsChanged = maxBlockSize != this->maxvs;
        const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;
    
        if (sampleRateChanged || maxvsChanged) {
            this->vs = maxBlockSize;
            this->maxvs = maxBlockSize;
            this->sr = sampleRate;
            this->invsr = 1 / sampleRate;
        }
    
        this->lores_01_dspsetup(forceDSPSetup);
        this->gen_01_dspsetup(forceDSPSetup);
        this->numbertilde_01_dspsetup(forceDSPSetup);
        this->scopetilde_01_dspsetup(forceDSPSetup);
    
        if (sampleRateChanged)
            this->onSampleRateChanged(sampleRate);
    }
    
    void setProbingTarget(MessageTag id) {
        switch (id) {
        default:
            {
            this->setProbingIndex(-1);
            break;
            }
        }
    }
    
    void setProbingIndex(ProbingIndex ) {}
    
    Index getProbingChannels(MessageTag outletId) const {
        RNBO_UNUSED(outletId);
        return 0;
    }
    
    void setVoiceIndex(Index index)  {
        this->_voiceIndex = index;
    }
    
    void setNoteNumber(Int noteNumber)  {
        this->_noteNumber = noteNumber;
    }
    
    Index getIsMuted()  {
        return this->isMuted;
    }
    
    void setIsMuted(Index v)  {
        this->isMuted = v;
    }
    
    Index getPatcherSerial() const {
        return 0;
    }
    
    void getState(PatcherStateInterface& ) {}
    
    void setState() {}
    
    void getPreset(PatcherStateInterface& ) {}
    
    void processTempoEvent(MillisecondTime , Tempo ) {}
    
    void processTransportEvent(MillisecondTime , TransportState ) {}
    
    void processBeatTimeEvent(MillisecondTime , BeatTime ) {}
    
    void onSampleRateChanged(double ) {}
    
    void processTimeSignatureEvent(MillisecondTime , int , int ) {}
    
    void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
        this->updateTime(time);
    
        switch (index) {
        case 0:
            {
            this->param_01_value_set(v);
            break;
            }
        case 1:
            {
            this->param_02_value_set(v);
            break;
            }
        case 2:
            {
            this->param_03_value_set(v);
            break;
            }
        case 3:
            {
            this->param_04_value_set(v);
            break;
            }
        case 4:
            {
            this->param_05_value_set(v);
            break;
            }
        }
    }
    
    void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValue(index, value, time);
    }
    
    void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
        this->setParameterValueNormalized(index, value, time);
    }
    
    ParameterValue getParameterValue(ParameterIndex index)  {
        switch (index) {
        case 0:
            {
            return this->param_01_value;
            }
        case 1:
            {
            return this->param_02_value;
            }
        case 2:
            {
            return this->param_03_value;
            }
        case 3:
            {
            return this->param_04_value;
            }
        case 4:
            {
            return this->param_05_value;
            }
        default:
            {
            return 0;
            }
        }
    }
    
    ParameterValue getPolyParameterValue(PatcherInterface** voices, ParameterIndex index)  {
        switch (index) {
        default:
            {
            return voices[0]->getParameterValue(index);
            }
        }
    }
    
    void setPolyParameterValue(
        PatcherInterface** voices,
        ParameterIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        switch (index) {
        default:
            {
            for (Index i = 0; i < 8; i++)
                voices[i]->setParameterValue(index, value, time);
            }
        }
    }
    
    ParameterIndex getNumSignalInParameters() const {
        return 0;
    }
    
    ParameterIndex getNumSignalOutParameters() const {
        return 0;
    }
    
    ParameterIndex getNumParameters() const {
        return 5;
    }
    
    ConstCharPointer getParameterName(ParameterIndex index) const {
        switch (index) {
        case 0:
            {
            return "cutOff";
            }
        case 1:
            {
            return "dampen";
            }
        case 2:
            {
            return "feedback";
            }
        case 3:
            {
            return "dispersion";
            }
        case 4:
            {
            return "tuning";
            }
        default:
            {
            return "bogus";
            }
        }
    }
    
    ConstCharPointer getParameterId(ParameterIndex index) const {
        switch (index) {
        case 0:
            {
            return "poly/cutOff";
            }
        case 1:
            {
            return "poly/dampen";
            }
        case 2:
            {
            return "poly/feedback";
            }
        case 3:
            {
            return "poly/dispersion";
            }
        case 4:
            {
            return "poly/tuning";
            }
        default:
            {
            return "bogus";
            }
        }
    }
    
    void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
        {
            switch (index) {
            case 0:
                {
                info->type = ParameterTypeNumber;
                info->initialValue = 2000;
                info->min = 200;
                info->max = 10000;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
                }
            case 1:
                {
                info->type = ParameterTypeNumber;
                info->initialValue = 0.003;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
                }
            case 2:
                {
                info->type = ParameterTypeNumber;
                info->initialValue = 0.99;
                info->min = 0;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
                }
            case 3:
                {
                info->type = ParameterTypeNumber;
                info->initialValue = 1;
                info->min = -1;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
                }
            case 4:
                {
                info->type = ParameterTypeNumber;
                info->initialValue = 0;
                info->min = -1;
                info->max = 1;
                info->exponent = 1;
                info->steps = 0;
                info->debug = false;
                info->saveable = true;
                info->transmittable = true;
                info->initialized = true;
                info->visible = false;
                info->displayName = "";
                info->unit = "";
                info->ioType = IOTypeUndefined;
                info->signalIndex = INVALID_INDEX;
                break;
                }
            }
        }
    }
    
    void sendParameter(ParameterIndex index, bool ignoreValue) {
        if (this->_voiceIndex == 1)
            this->getPatcher()->sendParameter(index + this->parameterOffset, ignoreValue);
    }
    
    void sendPolyParameter(ParameterIndex index, Index voiceIndex, bool ignoreValue) {
        this->getPatcher()->sendParameter(index + this->parameterOffset + voiceIndex - 1, ignoreValue);
    }
    
    void setParameterOffset(ParameterIndex offset) {
        this->parameterOffset = offset;
    }
    
    ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
        if (steps == 1) {
            if (normalizedValue > 0) {
                normalizedValue = 1.;
            }
        } else {
            ParameterValue oneStep = (number)1. / (steps - 1);
            ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
            normalizedValue = numberOfSteps * oneStep;
        }
    
        return normalizedValue;
    }
    
    ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 1:
        case 2:
            {
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - 0) / (1 - 0);
                return normalizedValue;
            }
            }
        case 0:
            {
            {
                value = (value < 200 ? 200 : (value > 10000 ? 10000 : value));
                ParameterValue normalizedValue = (value - 200) / (10000 - 200);
                return normalizedValue;
            }
            }
        case 3:
        case 4:
            {
            {
                value = (value < -1 ? -1 : (value > 1 ? 1 : value));
                ParameterValue normalizedValue = (value - -1) / (1 - -1);
                return normalizedValue;
            }
            }
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
        value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
        switch (index) {
        case 1:
        case 2:
            {
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 0 + value * (1 - 0);
                }
            }
            }
        case 0:
            {
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return 200 + value * (10000 - 200);
                }
            }
            }
        case 3:
        case 4:
            {
            {
                value = (value < 0 ? 0 : (value > 1 ? 1 : value));
    
                {
                    return -1 + value * (1 - -1);
                }
            }
            }
        default:
            {
            return value;
            }
        }
    }
    
    ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
        switch (index) {
        case 0:
            {
            return this->param_01_value_constrain(value);
            }
        case 1:
            {
            return this->param_02_value_constrain(value);
            }
        case 2:
            {
            return this->param_03_value_constrain(value);
            }
        case 3:
            {
            return this->param_04_value_constrain(value);
            }
        case 4:
            {
            return this->param_05_value_constrain(value);
            }
        default:
            {
            return value;
            }
        }
    }
    
    void scheduleParamInit(ParameterIndex index, Index order) {
        this->getPatcher()->scheduleParamInit(index + this->parameterOffset, order);
    }
    
    void processClockEvent(MillisecondTime time, ClockId index, bool hasValue, ParameterValue value) {
        RNBO_UNUSED(hasValue);
        this->updateTime(time);
    
        switch (index) {
        case 861222235:
            {
            this->curve_01_target_bang();
            break;
            }
        case -1494586265:
            {
            this->numbertilde_01_value_set(value);
            break;
            }
        }
    }
    
    void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}
    
    void processOutletEvent(
        EngineLink* sender,
        OutletIndex index,
        ParameterValue value,
        MillisecondTime time
    ) {
        this->updateTime(time);
        this->processOutletAtCurrentTime(sender, index, value);
    }
    
    void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            {
            if (TAG("poly/number~_obj-30") == objectId)
                this->numbertilde_01_sig_number_set(payload);
    
            break;
            }
        case TAG("mode"):
            {
            if (TAG("poly/number~_obj-30") == objectId)
                this->numbertilde_01_mode_set(payload);
    
            break;
            }
        }
    }
    
    void processListMessage(
        MessageTag tag,
        MessageTag objectId,
        MillisecondTime time,
        const list& payload
    ) {
        this->updateTime(time);
    
        switch (tag) {
        case TAG("sig"):
            {
            if (TAG("poly/number~_obj-30") == objectId)
                this->numbertilde_01_sig_list_set(payload);
    
            break;
            }
        }
    }
    
    void processBangMessage(MessageTag , MessageTag , MillisecondTime ) {}
    
    MessageTagInfo resolveTag(MessageTag tag) const {
        switch (tag) {
        case TAG("monitor"):
            {
            return "monitor";
            }
        case TAG("poly/number~_obj-30"):
            {
            return "poly/number~_obj-30";
            }
        case TAG("assign"):
            {
            return "assign";
            }
        case TAG("setup"):
            {
            return "setup";
            }
        case TAG("poly/scope~_obj-28"):
            {
            return "poly/scope~_obj-28";
            }
        case TAG("sig"):
            {
            return "sig";
            }
        case TAG("mode"):
            {
            return "mode";
            }
        }
    
        return nullptr;
    }
    
    DataRef* getDataRef(DataRefIndex index)  {
        switch (index) {
        case 1:
            {
            return addressOf(this->gen_01_delay_1_bufferobj);
            break;
            }
        default:
            {
            return nullptr;
            }
        }
    }
    
    DataRefIndex getNumDataRefs() const {
        return 1;
    }
    
    void fillDataRef(DataRefIndex , DataRef& ) {}
    
    void zeroDataRef(DataRef& ref) {
        ref->setZero();
    }
    
    void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
        this->updateTime(time);
    
        if (index == 0) {
            this->mtof_01_innerMtoF_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultMtofLookupTable256);
            this->gen_01_mtof_0_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultMtofLookupTable256);
        }
    
        if (index == 1) {
            this->gen_01_delay_1_buffer = new Float64Buffer(this->gen_01_delay_1_bufferobj);
        }
    }
    
    void initialize() {
        this->gen_01_delay_1_bufferobj = initDataRef("gen_01_delay_1_bufferobj", true, nullptr, "buffer~");
        this->assign_defaults();
        this->setState();
        this->mtof_01_innerMtoF_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultMtofLookupTable256);
        this->gen_01_mtof_0_buffer = new Float64Buffer(this->getPatcher()->RNBODefaultMtofLookupTable256);
        this->gen_01_delay_1_bufferobj->setIndex(1);
        this->gen_01_delay_1_buffer = new Float64Buffer(this->gen_01_delay_1_bufferobj);
    }
    
    protected:
    
    void numbertilde_01_sig_number_set(number v) {
        this->numbertilde_01_outValue = v;
    }
    
    void numbertilde_01_sig_list_set(const list& v) {
        this->numbertilde_01_outValue = v[0];
    }
    
    void numbertilde_01_mode_set(number v) {
        if (v == 1) {
            this->numbertilde_01_currentMode = 0;
        } else if (v == 2) {
            this->numbertilde_01_currentMode = 1;
        }
    }
    
    void param_01_value_set(number v) {
        v = this->param_01_value_constrain(v);
        this->param_01_value = v;
        this->sendParameter(0, false);
    
        if (this->param_01_value != this->param_01_lastValue) {
            this->getEngine()->presetTouched();
            this->param_01_lastValue = this->param_01_value;
        }
    
        this->lores_01_cutoff_set(v);
    }
    
    void param_02_value_set(number v) {
        v = this->param_02_value_constrain(v);
        this->param_02_value = v;
        this->sendParameter(1, false);
    
        if (this->param_02_value != this->param_02_lastValue) {
            this->getEngine()->presetTouched();
            this->param_02_lastValue = this->param_02_value;
        }
    
        this->gen_01_dampen_set(v);
    }
    
    void param_03_value_set(number v) {
        v = this->param_03_value_constrain(v);
        this->param_03_value = v;
        this->sendParameter(2, false);
    
        if (this->param_03_value != this->param_03_lastValue) {
            this->getEngine()->presetTouched();
            this->param_03_lastValue = this->param_03_value;
        }
    
        this->gen_01_feedback_set(v);
    }
    
    void param_04_value_set(number v) {
        v = this->param_04_value_constrain(v);
        this->param_04_value = v;
        this->sendParameter(3, false);
    
        if (this->param_04_value != this->param_04_lastValue) {
            this->getEngine()->presetTouched();
            this->param_04_lastValue = this->param_04_value;
        }
    
        this->gen_01_a_set(v);
    }
    
    void param_05_value_set(number v) {
        v = this->param_05_value_constrain(v);
        this->param_05_value = v;
        this->sendParameter(4, false);
    
        if (this->param_05_value != this->param_05_lastValue) {
            this->getEngine()->presetTouched();
            this->param_05_lastValue = this->param_05_value;
        }
    
        this->gen_01_b_set(v);
    }
    
    void curve_01_target_bang() {}
    
    void numbertilde_01_value_set(number ) {}
    
    number msToSamps(MillisecondTime ms, number sampleRate) {
        return ms * sampleRate * 0.001;
    }
    
    MillisecondTime sampsToMs(SampleIndex samps) {
        return samps * (this->invsr * 1000);
    }
    
    Index getMaxBlockSize() const {
        return this->maxvs;
    }
    
    number getSampleRate() const {
        return this->sr;
    }
    
    bool hasFixedVectorSize() const {
        return false;
    }
    
    Index getNumInputChannels() const {
        return 0;
    }
    
    Index getNumOutputChannels() const {
        return 2;
    }
    
    void initializeObjects() {
        this->mtof_01_innerScala_init();
        this->mtof_01_init();
        this->gen_01_history_6_init();
        this->gen_01_history_5_init();
        this->gen_01_history_4_init();
        this->gen_01_history_3_init();
        this->gen_01_history_2_init();
        this->gen_01_delay_1_init();
        this->numbertilde_01_init();
    }
    
    void sendOutlet(OutletIndex index, ParameterValue value) {
        this->getEngine()->sendOutlet(this, index, value);
    }
    
    void startup() {}
    
    void allocateDataRefs() {
        this->mtof_01_innerMtoF_buffer->requestSize(65536, 1);
        this->mtof_01_innerMtoF_buffer->setSampleRate(this->sr);
        this->gen_01_mtof_0_buffer->requestSize(65536, 1);
        this->gen_01_mtof_0_buffer->setSampleRate(this->sr);
        this->mtof_01_innerMtoF_buffer = this->mtof_01_innerMtoF_buffer->allocateIfNeeded();
        this->gen_01_mtof_0_buffer = this->gen_01_mtof_0_buffer->allocateIfNeeded();
        this->gen_01_delay_1_buffer = this->gen_01_delay_1_buffer->allocateIfNeeded();
    
        if (this->gen_01_delay_1_bufferobj->hasRequestedSize()) {
            if (this->gen_01_delay_1_bufferobj->wantsFill())
                this->zeroDataRef(this->gen_01_delay_1_bufferobj);
    
            this->getEngine()->sendDataRefUpdated(1);
        }
    }
    
    static number param_01_value_constrain(number v) {
        v = (v > 10000 ? 10000 : (v < 200 ? 200 : v));
        return v;
    }
    
    void lores_01_cutoff_set(number v) {
        this->lores_01_cutoff = v;
    }
    
    static number param_02_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
        return v;
    }
    
    static number gen_01_dampen_constrain(number v) {
        if (v < 0)
            v = 0;
    
        if (v > 1)
            v = 1;
    
        return v;
    }
    
    void gen_01_dampen_set(number v) {
        v = this->gen_01_dampen_constrain(v);
        this->gen_01_dampen = v;
    }
    
    static number param_03_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < 0 ? 0 : v));
        return v;
    }
    
    static number gen_01_feedback_constrain(number v) {
        if (v < 0)
            v = 0;
    
        if (v > 1)
            v = 1;
    
        return v;
    }
    
    void gen_01_feedback_set(number v) {
        v = this->gen_01_feedback_constrain(v);
        this->gen_01_feedback = v;
    }
    
    static number param_04_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < -1 ? -1 : v));
        return v;
    }
    
    void gen_01_a_set(number v) {
        this->gen_01_a = v;
    }
    
    static number param_05_value_constrain(number v) {
        v = (v > 1 ? 1 : (v < -1 ? -1 : v));
        return v;
    }
    
    void gen_01_b_set(number v) {
        this->gen_01_b = v;
    }
    
    void notein_01_outchannel_set(number ) {}
    
    void notein_01_releasevelocity_set(number ) {}
    
    void stripnote_01_velocity_set(number v) {
        this->stripnote_01_velocity = v;
    }
    
    void notein_01_velocity_set(number v) {
        this->stripnote_01_velocity_set(v);
    }
    
    void stripnote_01_velocity_out_set(number ) {}
    
    void gen_01_note_set(number v) {
        this->gen_01_note = v;
    }
    
    void trigger_02_out3_set(number v) {
        this->gen_01_note_set(v);
    }
    
    void expr_02_in2_set(number v) {
        this->expr_02_in2 = v;
    }
    
    void floatnum_01_out_set(number v) {
        this->expr_02_in2_set(v);
    }
    
    void floatnum_01_input_bang_bang() {
        this->floatnum_01_out_set(this->floatnum_01_stored);
    }
    
    void trigger_02_out2_bang() {
        this->floatnum_01_input_bang_bang();
    }
    
    void pack_01_in5_set(number v) {
        this->pack_01_in5 = v;
    }
    
    void pack_01_in2_set(number v) {
        this->pack_01_in2 = v;
    }
    
    void curve_01_dest_set(const list& v) {
        this->curve_01_dest = jsCreateListCopy(v);
    
        if ((bool)(v->length)) {
            if (v->length == 1 && this->curve_01_time == 0) {
                this->curve_01_activeRamps->length = 0;
                this->curve_01_currentValue = v[0];
            } else {
                number RFACTOR = (number)1 / (number)0.23;
                number PFACTOR = 5;
                number DIV_RFACTOR = (number)1 / (rnbo_exp(RFACTOR) - 1.);
                auto currentTime = this->currentsampletime();
                number lastRampEnd = currentTime;
                number lastRampValue = this->curve_01_currentValue;
                number rampEnd = currentTime - this->sampleOffsetIntoNextAudioBuffer;
                this->curve_01_samplecount = 0;
                this->curve_01_startvalue = this->curve_01_currentValue;
    
                for (Index i = 0; i < this->curve_01_activeRamps->length; i += 4) {
                    rampEnd = this->curve_01_activeRamps[(Index)(i + 1)];
    
                    if (rampEnd > currentTime) {
                        this->curve_01_activeRamps[(Index)i] = lastRampValue;
                        this->curve_01_activeRamps[(Index)(i + 1)] = currentTime;
                        this->curve_01_activeRamps->length = i + 4;
                        rampEnd = currentTime;
                    }
                }
    
                if (rampEnd < currentTime) {
                    this->curve_01_activeRamps->push(lastRampValue);
                    this->curve_01_activeRamps->push(currentTime);
                    this->curve_01_activeRamps->push(0);
                    this->curve_01_activeRamps->push(0);
                }
    
                for (Index i = 0; i < v->length; i += 3) {
                    number destinationValue = v[(Index)i];
                    number rampTimeInSamples;
                    number curveFactor;
    
                    if (v->length > i + 1) {
                        rampTimeInSamples = this->mstosamps(v[(Index)(i + 1)]);
                    } else {
                        rampTimeInSamples = this->mstosamps(this->curve_01_time);
                    }
    
                    if (rampTimeInSamples <= 0)
                        rampTimeInSamples = 1;
    
                    if (v->length > i + 2)
                        curveFactor = v[(Index)(i + 2)];
                    else
                        curveFactor = this->curve_01_curve;
    
                    number f = 1 - rnbo_abs(curveFactor);
                    number p = ((curveFactor >= 0. ? 1 : -1)) * PFACTOR * (rnbo_exp(f * RFACTOR) - 1.) * DIV_RFACTOR;
                    p = fixdenorm(p);
                    number div_erp = (number)1 / (rnbo_exp((number)1 / p) - 1.);
                    number div_durp = (number)1 / (rampTimeInSamples * p);
                    lastRampEnd += rampTimeInSamples;
                    this->curve_01_activeRamps->push(destinationValue);
                    this->curve_01_activeRamps->push(lastRampEnd);
                    this->curve_01_activeRamps->push(div_erp);
                    this->curve_01_activeRamps->push(div_durp);
                    lastRampValue = destinationValue;
                }
            }
        }
    }
    
    void pack_01_out_set(const list& v) {
        this->curve_01_dest_set(v);
    }
    
    void pack_01_in1_bang() {
        list array = list(0, 0, 0, 0, 0, 0);
        array[0] = this->pack_01_in1;
        array[1] = this->pack_01_in2;
        array[2] = this->pack_01_in3;
        array[3] = this->pack_01_in4;
        array[4] = this->pack_01_in5;
        array[5] = this->pack_01_in6;
        this->pack_01_out_set(array);
    }
    
    void trigger_01_out1_bang() {
        this->pack_01_in1_bang();
    }
    
    void trigger_01_input_number_set(number v) {
        RNBO_UNUSED(v);
        this->trigger_01_out1_bang();
    }
    
    void expr_01_out1_set(number v) {
        this->expr_01_out1 = v;
        this->pack_01_in5_set(this->expr_01_out1);
        this->pack_01_in2_set(this->expr_01_out1);
        this->trigger_01_input_number_set(this->expr_01_out1);
    }
    
    void expr_01_in1_set(number in1) {
        this->expr_01_in1 = in1;
        this->expr_01_out1_set(this->expr_01_in1 * this->expr_01_in2);//#map:*_obj-3:1
    }
    
    void expr_02_out1_set(number v) {
        this->expr_02_out1 = v;
        this->expr_01_in1_set(this->expr_02_out1);
    }
    
    void expr_02_in1_set(number in1) {
        this->expr_02_in1 = in1;
        this->expr_02_out1_set((this->expr_02_in1 == 0. ? 0. : this->expr_02_in2 / this->expr_02_in1));//#map:!/_obj-9:1
    }
    
    void mtof_01_out_set(const list& v) {
        {
            if (v->length > 1)
                this->expr_02_in2_set(v[1]);
    
            number converted = (v->length > 0 ? v[0] : 0);
            this->expr_02_in1_set(converted);
        }
    }
    
    void mtof_01_midivalue_set(const list& v) {
        this->mtof_01_midivalue = jsCreateListCopy(v);
        list tmp = list();
    
        for (int i = 0; i < this->mtof_01_midivalue->length; i++) {
            tmp->push(
                this->mtof_01_innerMtoF_next(this->mtof_01_midivalue[(Index)i], this->mtof_01_base)
            );
        }
    
        this->mtof_01_out_set(tmp);
    }
    
    void trigger_02_out1_set(number v) {
        {
            list converted = {v};
            this->mtof_01_midivalue_set(converted);
        }
    }
    
    void trigger_02_input_number_set(number v) {
        this->trigger_02_out3_set(trunc(v));
        this->trigger_02_out2_bang();
        this->trigger_02_out1_set(v);
    }
    
    void stripnote_01_notenumber_out_set(number v) {
        this->trigger_02_input_number_set(v);
    }
    
    void stripnote_01_notenumber_set(number v) {
        if ((bool)(this->stripnote_01_velocity)) {
            this->stripnote_01_velocity_out_set(this->stripnote_01_velocity);
            this->stripnote_01_notenumber_out_set(v);
        }
    }
    
    void notein_01_notenumber_set(number v) {
        this->stripnote_01_notenumber_set(v);
    }
    
    void notein_01_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
        RNBO_UNUSED(length);
        RNBO_UNUSED(port);
    
        if (channel == this->notein_01_channel || this->notein_01_channel == 0) {
            if (status == 144 || status == 128) {
                this->notein_01_outchannel_set(channel);
    
                if (status == 128) {
                    this->notein_01_releasevelocity_set(data[2]);
                    this->notein_01_velocity_set(0);
                } else {
                    this->notein_01_releasevelocity_set(0);
                    this->notein_01_velocity_set(data[2]);
                }
    
                this->notein_01_notenumber_set(data[1]);
            }
        }
    }
    
    void midiouthelper_midiout_set(number ) {}
    
    void curve_01_perform(SampleValue * out, Index n) {
        auto __curve_01_time = this->curve_01_time;
        auto __curve_01_startvalue = this->curve_01_startvalue;
        auto __curve_01_samplecount = this->curve_01_samplecount;
        auto __curve_01_currentValue = this->curve_01_currentValue;
        Index i = 0;
    
        if ((bool)(this->curve_01_activeRamps->length)) {
            while ((bool)(this->curve_01_activeRamps->length) && i < n) {
                number destinationValue = this->curve_01_activeRamps[0];
                number rampTimeInSamples = this->curve_01_activeRamps[1] - this->audioProcessSampleCount - i;
                number val = __curve_01_currentValue;
                number div_erp = this->curve_01_activeRamps[2];
                number div_durp = this->curve_01_activeRamps[3];
    
                while (rampTimeInSamples > 0 && i < n) {
                    number curv = (rnbo_exp(__curve_01_samplecount * div_durp) - 1) * div_erp;
                    val += __curve_01_startvalue - val + curv * (destinationValue - __curve_01_startvalue);
                    out[(Index)i] = val;
                    i++;
                    __curve_01_samplecount++;
                    rampTimeInSamples--;
                }
    
                if (rampTimeInSamples <= 0) {
                    __curve_01_samplecount = 0;
                    val = destinationValue;
                    __curve_01_startvalue = val;
                    this->curve_01_activeRamps->splice(0, 4);
    
                    if ((bool)(!(bool)(this->curve_01_activeRamps->length))) {
                        this->getEngine()->scheduleClockEventWithValue(
                            this,
                            861222235,
                            this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                            0
                        );;
    
                        __curve_01_time = 0;
                    }
                }
    
                __curve_01_currentValue = val;
            }
        }
    
        while (i < n) {
            out[(Index)i] = __curve_01_currentValue;
            i++;
        }
    
        this->curve_01_currentValue = __curve_01_currentValue;
        this->curve_01_samplecount = __curve_01_samplecount;
        this->curve_01_startvalue = __curve_01_startvalue;
        this->curve_01_time = __curve_01_time;
    }
    
    void lores_01_perform(
        number cutoff,
        number resonance,
        const SampleValue * input,
        SampleValue * out,
        Index n
    ) {
        RNBO_UNUSED(resonance);
        auto __lores_01_y_2 = this->lores_01_y_2;
        auto __lores_01_y_1 = this->lores_01_y_1;
        auto __lores_01_x_0 = this->lores_01_x_0;
        auto __lores_01_b_0 = this->lores_01_b_0;
        auto __lores_01_a_1 = this->lores_01_a_1;
        auto __lores_01_a_2 = this->lores_01_a_2;
        auto __lores_01_last_freq_calc = this->lores_01_last_freq_calc;
        auto __lores_01_last_res_calc = this->lores_01_last_res_calc;
        auto __lores_01_last_frequency = this->lores_01_last_frequency;
        auto __lores_01_last_resonance = this->lores_01_last_resonance;
    
        for (Index i = 0; i < n; i++) {
            if (0.2 != __lores_01_last_resonance || cutoff != __lores_01_last_frequency) {
                __lores_01_last_res_calc = rnbo_exp(0.2 * 0.125) * 0.882497;
                __lores_01_last_freq_calc = rnbo_cos(2.0 * pi01() / this->sr * cutoff);
                __lores_01_last_resonance = 0.2;
                __lores_01_last_frequency = cutoff;
            }
    
            number res_temp = __lores_01_last_res_calc;
            number freq_temp = __lores_01_last_freq_calc;
            __lores_01_a_2 = res_temp * res_temp;
            __lores_01_a_1 = -2.0 * res_temp * freq_temp;
            __lores_01_b_0 = 1.0 + __lores_01_a_1 + __lores_01_a_2;
            __lores_01_x_0 = input[(Index)i];
            number y_0 = __lores_01_x_0 * __lores_01_b_0 - __lores_01_y_1 * __lores_01_a_1 - __lores_01_y_2 * __lores_01_a_2;
            __lores_01_y_2 = __lores_01_y_1;
            __lores_01_y_1 = y_0;
            out[(Index)i] = y_0;
        }
    
        this->lores_01_last_resonance = __lores_01_last_resonance;
        this->lores_01_last_frequency = __lores_01_last_frequency;
        this->lores_01_last_res_calc = __lores_01_last_res_calc;
        this->lores_01_last_freq_calc = __lores_01_last_freq_calc;
        this->lores_01_a_2 = __lores_01_a_2;
        this->lores_01_a_1 = __lores_01_a_1;
        this->lores_01_b_0 = __lores_01_b_0;
        this->lores_01_x_0 = __lores_01_x_0;
        this->lores_01_y_1 = __lores_01_y_1;
        this->lores_01_y_2 = __lores_01_y_2;
    }
    
    void gen_01_perform(
        const Sample * in1,
        number feedback,
        number note,
        number dampen,
        number a,
        number b,
        SampleValue * out1,
        SampleValue * out2,
        Index n
    ) {
        auto __gen_01_history_6_value = this->gen_01_history_6_value;
        auto __gen_01_history_2_value = this->gen_01_history_2_value;
        auto __gen_01_history_4_value = this->gen_01_history_4_value;
        auto __gen_01_history_3_value = this->gen_01_history_3_value;
        auto __gen_01_history_5_value = this->gen_01_history_5_value;
        number rsub_13_8 = 1 - a;
        number mul_14_9 = rsub_13_8 * b;
        number mul_17_12 = a * -1;
        Index i;
    
        for (i = 0; i < n; i++) {
            number mtof_7_1 = this->gen_01_mtof_0_next(note, 440);
            number rdiv_8_2 = (mtof_7_1 == 0. ? 0. : this->samplerate() / mtof_7_1);
            out2[(Index)i] = rdiv_8_2;
            number tap_9_3 = this->gen_01_delay_1_read(rdiv_8_2, 0);
            out1[(Index)i] = tap_9_3;
            number dcblock_10_5 = this->gen_01_dcblock_4_next(tap_9_3, 0.9997);
            number mul_11_6 = __gen_01_history_5_value * -1;
            number mul_12_7 = __gen_01_history_3_value * a;
            number mul_15_10 = mul_11_6 * mul_14_9;
            number mul_16_11 = __gen_01_history_4_value * mul_14_9;
            number mul_18_13 = mul_17_12 * dcblock_10_5;
            number add_19_14 = mul_18_13 + __gen_01_history_2_value;
            number add_20_15 = add_19_14 + mul_16_11;
            number add_21_16 = mul_12_7 + add_20_15;
            number add_22_17 = add_21_16 + mul_15_10;
            number gen_23_18 = add_22_17;
            number history_2_next_24_19 = fixdenorm(__gen_01_history_4_value);
            number history_3_next_25_20 = fixdenorm(__gen_01_history_5_value);
            number history_4_next_26_21 = fixdenorm(dcblock_10_5);
            number history_5_next_27_22 = fixdenorm(add_22_17);
            number mul_28_23 = gen_23_18 * feedback;
            number mul_29_24 = __gen_01_history_6_value * -1;
            number mix_30_25 = mul_28_23 + dampen * (mul_29_24 - mul_28_23);
            number gen_31_26 = mix_30_25;
            number history_6_next_32_27 = fixdenorm(mix_30_25);
            this->gen_01_delay_1_write(in1[(Index)i] + gen_31_26);
            __gen_01_history_6_value = history_6_next_32_27;
            __gen_01_history_3_value = history_3_next_25_20;
            __gen_01_history_4_value = history_4_next_26_21;
            __gen_01_history_5_value = history_5_next_27_22;
            __gen_01_history_2_value = history_2_next_24_19;
            this->gen_01_delay_1_step();
        }
    
        this->gen_01_history_5_value = __gen_01_history_5_value;
        this->gen_01_history_3_value = __gen_01_history_3_value;
        this->gen_01_history_4_value = __gen_01_history_4_value;
        this->gen_01_history_2_value = __gen_01_history_2_value;
        this->gen_01_history_6_value = __gen_01_history_6_value;
    }
    
    void dspexpr_01_perform(const Sample * in1, number in2, number in3, SampleValue * out1, Index n) {
        RNBO_UNUSED(in3);
        RNBO_UNUSED(in2);
        Index i;
    
        for (i = 0; i < n; i++) {
            out1[(Index)i] = (in1[(Index)i] > 1 ? 1 : (in1[(Index)i] < -1 ? -1 : in1[(Index)i]));//#map:_###_obj_###_:1
        }
    }
    
    void signaladder_01_perform(
        const SampleValue * in1,
        const SampleValue * in2,
        SampleValue * out,
        Index n
    ) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out[(Index)i] = in1[(Index)i] + in2[(Index)i];
        }
    }
    
    void signaladder_02_perform(
        const SampleValue * in1,
        const SampleValue * in2,
        SampleValue * out,
        Index n
    ) {
        Index i;
    
        for (i = 0; i < n; i++) {
            out[(Index)i] = in1[(Index)i] + in2[(Index)i];
        }
    }
    
    void numbertilde_01_perform(const SampleValue * input_signal, SampleValue * output, Index n) {
        auto __numbertilde_01_currentIntervalInSamples = this->numbertilde_01_currentIntervalInSamples;
        auto __numbertilde_01_lastValue = this->numbertilde_01_lastValue;
        auto __numbertilde_01_currentInterval = this->numbertilde_01_currentInterval;
        auto __numbertilde_01_rampInSamples = this->numbertilde_01_rampInSamples;
        auto __numbertilde_01_outValue = this->numbertilde_01_outValue;
        auto __numbertilde_01_currentMode = this->numbertilde_01_currentMode;
        number monitorvalue = input_signal[0];
    
        for (Index i = 0; i < n; i++) {
            if (__numbertilde_01_currentMode == 0) {
                output[(Index)i] = this->numbertilde_01_smooth_next(
                    __numbertilde_01_outValue,
                    __numbertilde_01_rampInSamples,
                    __numbertilde_01_rampInSamples
                );
            } else {
                output[(Index)i] = input_signal[(Index)i];
            }
        }
    
        __numbertilde_01_currentInterval -= n;
    
        if (monitorvalue != __numbertilde_01_lastValue && __numbertilde_01_currentInterval <= 0) {
            __numbertilde_01_currentInterval = __numbertilde_01_currentIntervalInSamples;
    
            this->getEngine()->scheduleClockEventWithValue(
                this,
                -1494586265,
                this->sampsToMs((SampleIndex)(this->vs)) + this->_currentTime,
                monitorvalue
            );;
    
            __numbertilde_01_lastValue = monitorvalue;
    
            this->getEngine()->sendListMessage(
                TAG("monitor"),
                TAG("poly/number~_obj-30"),
                {monitorvalue},
                this->_currentTime
            );;
        }
    
        this->numbertilde_01_currentInterval = __numbertilde_01_currentInterval;
        this->numbertilde_01_lastValue = __numbertilde_01_lastValue;
    }
    
    void scopetilde_01_perform(const SampleValue * x, const SampleValue * y, Index n) {
        auto __scopetilde_01_ysign = this->scopetilde_01_ysign;
        auto __scopetilde_01_ymonitorvalue = this->scopetilde_01_ymonitorvalue;
        auto __scopetilde_01_xsign = this->scopetilde_01_xsign;
        auto __scopetilde_01_xmonitorvalue = this->scopetilde_01_xmonitorvalue;
        auto __scopetilde_01_mode = this->scopetilde_01_mode;
    
        for (Index i = 0; i < n; i++) {
            number xval = x[(Index)i];
            number yval = y[(Index)i];
    
            if (__scopetilde_01_mode == 1) {
                number xabsval = rnbo_abs(xval);
    
                if (xabsval > __scopetilde_01_xmonitorvalue) {
                    __scopetilde_01_xmonitorvalue = xabsval;
                    __scopetilde_01_xsign = (xval < 0 ? -1 : 1);
                }
    
                number yabsval = rnbo_abs(yval);
    
                if (yabsval > __scopetilde_01_ymonitorvalue) {
                    __scopetilde_01_ymonitorvalue = yabsval;
                    __scopetilde_01_ysign = (yval < 0 ? -1 : 1);
                }
            } else {
                __scopetilde_01_xmonitorvalue = xval;
                __scopetilde_01_xsign = 1;
                __scopetilde_01_ymonitorvalue = yval;
                __scopetilde_01_ysign = 1;
            }
    
            this->scopetilde_01_effectiveCount--;
    
            if (this->scopetilde_01_effectiveCount <= 0) {
                this->scopetilde_01_updateEffectiveCount();
                this->scopetilde_01_monitorbuffer->push(__scopetilde_01_xmonitorvalue * __scopetilde_01_xsign);
    
                if (__scopetilde_01_mode == 1)
                    __scopetilde_01_xmonitorvalue = 0;
    
                if (this->scopetilde_01_monitorbuffer->length >= 128 * (1 + 0)) {
                    this->getEngine()->sendListMessage(
                        TAG("monitor"),
                        TAG("poly/scope~_obj-28"),
                        this->scopetilde_01_monitorbuffer,
                        this->_currentTime
                    );;
    
                    this->scopetilde_01_monitorbuffer->length = 0;
                }
            }
        }
    
        this->scopetilde_01_xmonitorvalue = __scopetilde_01_xmonitorvalue;
        this->scopetilde_01_xsign = __scopetilde_01_xsign;
        this->scopetilde_01_ymonitorvalue = __scopetilde_01_ymonitorvalue;
        this->scopetilde_01_ysign = __scopetilde_01_ysign;
    }
    
    void stackprotect_perform(Index n) {
        RNBO_UNUSED(n);
        auto __stackprotect_count = this->stackprotect_count;
        __stackprotect_count = 0;
        this->stackprotect_count = __stackprotect_count;
    }
    
    void lores_01_resonance_setter(number v) {
        if (v < 0.0) {
            v = 0.0;
        } else if (v >= 1.0) {
            v = 1.0 - 1E-20;
        }
    
        this->lores_01_resonance = v;
    }
    
    number mtof_01_innerMtoF_next(number midivalue, number tuning) {
        if (midivalue == this->mtof_01_innerMtoF_lastInValue && tuning == this->mtof_01_innerMtoF_lastTuning)
            return this->mtof_01_innerMtoF_lastOutValue;
    
        this->mtof_01_innerMtoF_lastInValue = midivalue;
        this->mtof_01_innerMtoF_lastTuning = tuning;
        number result = 0;
    
        {
            result = rnbo_exp(.057762265 * (midivalue - 69.0));
        }
    
        this->mtof_01_innerMtoF_lastOutValue = tuning * result;
        return this->mtof_01_innerMtoF_lastOutValue;
    }
    
    void mtof_01_innerMtoF_reset() {
        this->mtof_01_innerMtoF_lastInValue = 0;
        this->mtof_01_innerMtoF_lastOutValue = 0;
        this->mtof_01_innerMtoF_lastTuning = 0;
    }
    
    void mtof_01_innerScala_mid(int v) {
        this->mtof_01_innerScala_kbmMid = v;
        this->mtof_01_innerScala_updateRefFreq();
    }
    
    void mtof_01_innerScala_ref(int v) {
        this->mtof_01_innerScala_kbmRefNum = v;
        this->mtof_01_innerScala_updateRefFreq();
    }
    
    void mtof_01_innerScala_base(number v) {
        this->mtof_01_innerScala_kbmRefFreq = v;
        this->mtof_01_innerScala_updateRefFreq();
    }
    
    void mtof_01_innerScala_init() {
        list sclValid = {
            12,
            100,
            0,
            200,
            0,
            300,
            0,
            400,
            0,
            500,
            0,
            600,
            0,
            700,
            0,
            800,
            0,
            900,
            0,
            1000,
            0,
            1100,
            0,
            2,
            1
        };
    
        this->mtof_01_innerScala_updateScale(sclValid);
    }
    
    void mtof_01_innerScala_update(list scale, list map) {
        if (scale->length > 0) {
            this->mtof_01_innerScala_updateScale(scale);
        }
    
        if (map->length > 0) {
            this->mtof_01_innerScala_updateMap(map);
        }
    }
    
    number mtof_01_innerScala_mtof(number note) {
        if ((bool)(this->mtof_01_innerScala_lastValid) && this->mtof_01_innerScala_lastNote == note) {
            return this->mtof_01_innerScala_lastFreq;
        }
    
        array<int, 2> degoct = this->mtof_01_innerScala_applyKBM(note);
        number out = 0;
    
        if (degoct[1] > 0) {
            out = this->mtof_01_innerScala_applySCL(degoct[0], fract(note), this->mtof_01_innerScala_refFreq);
        }
    
        this->mtof_01_innerScala_updateLast(note, out);
        return out;
    }
    
    number mtof_01_innerScala_ftom(number hz) {
        if (hz <= 0.0) {
            return 0.0;
        }
    
        if ((bool)(this->mtof_01_innerScala_lastValid) && this->mtof_01_innerScala_lastFreq == hz) {
            return this->mtof_01_innerScala_lastNote;
        }
    
        array<number, 2> df = this->mtof_01_innerScala_hztodeg(hz);
        int degree = (int)(df[0]);
        number frac = df[1];
        number out = 0;
    
        if (this->mtof_01_innerScala_kbmSize == 0) {
            out = this->mtof_01_innerScala_kbmMid + degree;
        } else {
            array<int, 2> octdeg = this->mtof_01_innerScala_octdegree(degree, this->mtof_01_innerScala_kbmOctaveDegree);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
            Index entry = 0;
    
            for (Index i = 0; i < this->mtof_01_innerScala_kbmMapSize; i++) {
                if (index == this->mtof_01_innerScala_kbmValid[(Index)(i + this->mtof_01_innerScala_KBM_MAP_OFFSET)]) {
                    entry = i;
                    break;
                }
            }
    
            out = oct * this->mtof_01_innerScala_kbmSize + entry + this->mtof_01_innerScala_kbmMid;
        }
    
        out = out + frac;
        this->mtof_01_innerScala_updateLast(out, hz);
        return this->mtof_01_innerScala_lastNote;
    }
    
    int mtof_01_innerScala_updateScale(list scl) {
        if (scl->length > 1 && scl[0] * 2 + 1 == scl->length) {
            this->mtof_01_innerScala_lastValid = false;
            this->mtof_01_innerScala_sclExpMul = {};
            number last = 1;
    
            for (Index i = 1; i < scl->length; i += 2) {
                const number c = (const number)(scl[(Index)(i + 0)]);
                const number d = (const number)(scl[(Index)(i + 1)]);
    
                if (d <= 0) {
                    last = c / (number)1200;
                } else {
                    last = rnbo_log2(c / d);
                }
    
                this->mtof_01_innerScala_sclExpMul->push(last);
            }
    
            this->mtof_01_innerScala_sclOctaveMul = last;
            this->mtof_01_innerScala_sclEntryCount = (int)(this->mtof_01_innerScala_sclExpMul->length);
            this->mtof_01_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    int mtof_01_innerScala_updateMap(list kbm) {
        if (kbm->length == 1 && kbm[0] == 0.0) {
            kbm = {0.0, 0.0, 0.0, 60.0, 69.0, 440.0};
        }
    
        if (kbm->length >= 6 && kbm[0] >= 0.0) {
            this->mtof_01_innerScala_lastValid = false;
            Index size = (Index)(kbm[0]);
            int octave = 12;
    
            if (kbm->length > 6) {
                octave = (int)(kbm[6]);
            }
    
            if (size > 0 && kbm->length < this->mtof_01_innerScala_KBM_MAP_OFFSET) {
                return 0;
            }
    
            this->mtof_01_innerScala_kbmSize = (int)(size);
            this->mtof_01_innerScala_kbmMin = (int)(kbm[1]);
            this->mtof_01_innerScala_kbmMax = (int)(kbm[2]);
            this->mtof_01_innerScala_kbmMid = (int)(kbm[3]);
            this->mtof_01_innerScala_kbmRefNum = (int)(kbm[4]);
            this->mtof_01_innerScala_kbmRefFreq = kbm[5];
            this->mtof_01_innerScala_kbmOctaveDegree = octave;
            this->mtof_01_innerScala_kbmValid = kbm;
            this->mtof_01_innerScala_kbmMapSize = (kbm->length - this->mtof_01_innerScala_KBM_MAP_OFFSET > kbm->length ? kbm->length : (kbm->length - this->mtof_01_innerScala_KBM_MAP_OFFSET < 0 ? 0 : kbm->length - this->mtof_01_innerScala_KBM_MAP_OFFSET));
            this->mtof_01_innerScala_updateRefFreq();
            return 1;
        }
    
        return 0;
    }
    
    void mtof_01_innerScala_updateLast(number note, number freq) {
        this->mtof_01_innerScala_lastValid = true;
        this->mtof_01_innerScala_lastNote = note;
        this->mtof_01_innerScala_lastFreq = freq;
    }
    
    array<number, 2> mtof_01_innerScala_hztodeg(number hz) {
        number hza = rnbo_abs(hz);
    
        number octave = rnbo_floor(
            rnbo_log2(hza / this->mtof_01_innerScala_refFreq) / this->mtof_01_innerScala_sclOctaveMul
        );
    
        int i = 0;
        number frac = 0;
        number n = 0;
    
        for (; i < this->mtof_01_innerScala_sclEntryCount; i++) {
            number c = this->mtof_01_innerScala_applySCLOctIndex(octave, i + 0, 0.0, this->mtof_01_innerScala_refFreq);
            n = this->mtof_01_innerScala_applySCLOctIndex(octave, i + 1, 0.0, this->mtof_01_innerScala_refFreq);
    
            if (c <= hza && hza < n) {
                if (c != hza) {
                    frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
                }
    
                break;
            }
        }
    
        if (i == this->mtof_01_innerScala_sclEntryCount && n != hza) {
            number c = n;
            n = this->mtof_01_innerScala_applySCLOctIndex(octave + 1, 0, 0.0, this->mtof_01_innerScala_refFreq);
            frac = rnbo_log2(hza / c) / rnbo_log2(n / c);
        }
    
        number deg = i + octave * this->mtof_01_innerScala_sclEntryCount;
    
        {
            deg = rnbo_fround((deg + frac) * 1 / (number)1) * 1;
            frac = 0.0;
        }
    
        return {deg, frac};
    }
    
    array<int, 2> mtof_01_innerScala_octdegree(int degree, int count) {
        int octave = 0;
        int index = 0;
    
        if (degree < 0) {
            octave = -(1 + (-1 - degree) / count);
            index = -degree % count;
    
            if (index > 0) {
                index = count - index;
            }
        } else {
            octave = degree / count;
            index = degree % count;
        }
    
        return {octave, index};
    }
    
    array<int, 2> mtof_01_innerScala_applyKBM(number note) {
        if ((this->mtof_01_innerScala_kbmMin == this->mtof_01_innerScala_kbmMax && this->mtof_01_innerScala_kbmMax == 0) || (note >= this->mtof_01_innerScala_kbmMin && note <= this->mtof_01_innerScala_kbmMax)) {
            int degree = (int)(rnbo_floor(note - this->mtof_01_innerScala_kbmMid));
    
            if (this->mtof_01_innerScala_kbmSize == 0) {
                return {degree, 1};
            }
    
            array<int, 2> octdeg = this->mtof_01_innerScala_octdegree(degree, this->mtof_01_innerScala_kbmSize);
            int octave = (int)(octdeg[0]);
            Index index = (Index)(octdeg[1]);
    
            if (this->mtof_01_innerScala_kbmMapSize > index) {
                degree = (int)(this->mtof_01_innerScala_kbmValid[(Index)(this->mtof_01_innerScala_KBM_MAP_OFFSET + index)]);
    
                if (degree >= 0) {
                    return {degree + octave * this->mtof_01_innerScala_kbmOctaveDegree, 1};
                }
            }
        }
    
        return {-1, 0};
    }
    
    number mtof_01_innerScala_applySCL(int degree, number frac, number refFreq) {
        array<int, 2> octdeg = this->mtof_01_innerScala_octdegree(degree, this->mtof_01_innerScala_sclEntryCount);
        return this->mtof_01_innerScala_applySCLOctIndex(octdeg[0], octdeg[1], frac, refFreq);
    }
    
    number mtof_01_innerScala_applySCLOctIndex(number octave, int index, number frac, number refFreq) {
        number p = 0;
    
        if (index > 0) {
            p = this->mtof_01_innerScala_sclExpMul[(Index)(index - 1)];
        }
    
        if (frac > 0) {
            p = this->linearinterp(frac, p, this->mtof_01_innerScala_sclExpMul[(Index)index]);
        } else if (frac < 0) {
            p = this->linearinterp(-frac, this->mtof_01_innerScala_sclExpMul[(Index)index], p);
        }
    
        return refFreq * rnbo_pow(2, p + octave * this->mtof_01_innerScala_sclOctaveMul);
    }
    
    void mtof_01_innerScala_updateRefFreq() {
        this->mtof_01_innerScala_lastValid = false;
        int refOffset = (int)(this->mtof_01_innerScala_kbmRefNum - this->mtof_01_innerScala_kbmMid);
    
        if (refOffset == 0) {
            this->mtof_01_innerScala_refFreq = this->mtof_01_innerScala_kbmRefFreq;
        } else {
            int base = (int)(this->mtof_01_innerScala_kbmSize);
    
            if (base < 1) {
                base = this->mtof_01_innerScala_sclEntryCount;
            }
    
            array<int, 2> octdeg = this->mtof_01_innerScala_octdegree(refOffset, base);
            number oct = (number)(octdeg[0]);
            int index = (int)(octdeg[1]);
    
            if (base > 0) {
                oct = oct + rnbo_floor(index / base);
                index = index % base;
            }
    
            if (index >= 0 && index < this->mtof_01_innerScala_kbmSize) {
                if (index < this->mtof_01_innerScala_kbmMapSize) {
                    index = (int)(this->mtof_01_innerScala_kbmValid[(Index)((Index)(index) + this->mtof_01_innerScala_KBM_MAP_OFFSET)]);
                } else {
                    index = -1;
                }
            }
    
            if (index < 0 || index > this->mtof_01_innerScala_sclExpMul->length)
                {} else {
                number p = 0;
    
                if (index > 0) {
                    p = this->mtof_01_innerScala_sclExpMul[(Index)(index - 1)];
                }
    
                this->mtof_01_innerScala_refFreq = this->mtof_01_innerScala_kbmRefFreq / rnbo_pow(2, p + oct * this->mtof_01_innerScala_sclOctaveMul);
            }
        }
    }
    
    void mtof_01_innerScala_reset() {
        this->mtof_01_innerScala_internal = true;
        this->mtof_01_innerScala_lastValid = false;
        this->mtof_01_innerScala_lastNote = 0;
        this->mtof_01_innerScala_lastFreq = 0;
        this->mtof_01_innerScala_sclEntryCount = 0;
        this->mtof_01_innerScala_sclOctaveMul = 1;
        this->mtof_01_innerScala_sclExpMul = {};
        this->mtof_01_innerScala_kbmValid = {0, 0, 0, 60, 69, 440};
        this->mtof_01_innerScala_kbmMid = 60;
        this->mtof_01_innerScala_kbmRefNum = 69;
        this->mtof_01_innerScala_kbmRefFreq = 440;
        this->mtof_01_innerScala_kbmSize = 0;
        this->mtof_01_innerScala_kbmMin = 0;
        this->mtof_01_innerScala_kbmMax = 0;
        this->mtof_01_innerScala_kbmOctaveDegree = 12;
        this->mtof_01_innerScala_kbmMapSize = 0;
        this->mtof_01_innerScala_refFreq = 261.63;
    }
    
    void mtof_01_init() {
        this->mtof_01_innerScala_update(this->mtof_01_scale, this->mtof_01_map);
    }
    
    number gen_01_history_6_getvalue() {
        return this->gen_01_history_6_value;
    }
    
    void gen_01_history_6_setvalue(number val) {
        this->gen_01_history_6_value = val;
    }
    
    void gen_01_history_6_reset() {
        this->gen_01_history_6_value = 0;
    }
    
    void gen_01_history_6_init() {
        this->gen_01_history_6_value = 0;
    }
    
    number gen_01_history_5_getvalue() {
        return this->gen_01_history_5_value;
    }
    
    void gen_01_history_5_setvalue(number val) {
        this->gen_01_history_5_value = val;
    }
    
    void gen_01_history_5_reset() {
        this->gen_01_history_5_value = 0;
    }
    
    void gen_01_history_5_init() {
        this->gen_01_history_5_value = 0;
    }
    
    number gen_01_history_4_getvalue() {
        return this->gen_01_history_4_value;
    }
    
    void gen_01_history_4_setvalue(number val) {
        this->gen_01_history_4_value = val;
    }
    
    void gen_01_history_4_reset() {
        this->gen_01_history_4_value = 0;
    }
    
    void gen_01_history_4_init() {
        this->gen_01_history_4_value = 0;
    }
    
    number gen_01_history_3_getvalue() {
        return this->gen_01_history_3_value;
    }
    
    void gen_01_history_3_setvalue(number val) {
        this->gen_01_history_3_value = val;
    }
    
    void gen_01_history_3_reset() {
        this->gen_01_history_3_value = 0;
    }
    
    void gen_01_history_3_init() {
        this->gen_01_history_3_value = 0;
    }
    
    number gen_01_history_2_getvalue() {
        return this->gen_01_history_2_value;
    }
    
    void gen_01_history_2_setvalue(number val) {
        this->gen_01_history_2_value = val;
    }
    
    void gen_01_history_2_reset() {
        this->gen_01_history_2_value = 0;
    }
    
    void gen_01_history_2_init() {
        this->gen_01_history_2_value = 0;
    }
    
    void gen_01_delay_1_step() {
        this->gen_01_delay_1_reader++;
    
        if (this->gen_01_delay_1_reader >= (int)(this->gen_01_delay_1_buffer->getSize()))
            this->gen_01_delay_1_reader = 0;
    }
    
    number gen_01_delay_1_read(number size, Int interp) {
        RNBO_UNUSED(interp);
    
        {
            number r = (int)(this->gen_01_delay_1_buffer->getSize()) + this->gen_01_delay_1_reader - ((size > this->gen_01_delay_1__maxdelay ? this->gen_01_delay_1__maxdelay : (size < (this->gen_01_delay_1_reader != this->gen_01_delay_1_writer) ? this->gen_01_delay_1_reader != this->gen_01_delay_1_writer : size)));
            long index1 = (long)(rnbo_floor(r));
            number frac = r - index1;
            long index2 = (long)(index1 + 1);
    
            return this->linearinterp(frac, this->gen_01_delay_1_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->gen_01_delay_1_wrap))
            ), this->gen_01_delay_1_buffer->getSample(
                0,
                (Index)((BinOpInt)((BinOpInt)index2 & (BinOpInt)this->gen_01_delay_1_wrap))
            ));
        }
    
        number r = (int)(this->gen_01_delay_1_buffer->getSize()) + this->gen_01_delay_1_reader - ((size > this->gen_01_delay_1__maxdelay ? this->gen_01_delay_1__maxdelay : (size < (this->gen_01_delay_1_reader != this->gen_01_delay_1_writer) ? this->gen_01_delay_1_reader != this->gen_01_delay_1_writer : size)));
        long index1 = (long)(rnbo_floor(r));
    
        return this->gen_01_delay_1_buffer->getSample(
            0,
            (Index)((BinOpInt)((BinOpInt)index1 & (BinOpInt)this->gen_01_delay_1_wrap))
        );
    }
    
    void gen_01_delay_1_write(number v) {
        this->gen_01_delay_1_writer = this->gen_01_delay_1_reader;
        this->gen_01_delay_1_buffer[(Index)this->gen_01_delay_1_writer] = v;
    }
    
    number gen_01_delay_1_next(number v, int size) {
        number effectiveSize = (size == -1 ? this->gen_01_delay_1__maxdelay : size);
        number val = this->gen_01_delay_1_read(effectiveSize, 0);
        this->gen_01_delay_1_write(v);
        this->gen_01_delay_1_step();
        return val;
    }
    
    array<Index, 2> gen_01_delay_1_calcSizeInSamples() {
        number sizeInSamples = 0;
        Index allocatedSizeInSamples = 0;
    
        {
            sizeInSamples = this->gen_01_delay_1_evaluateSizeExpr(this->samplerate(), this->vectorsize());
            this->gen_01_delay_1_sizemode = 0;
        }
    
        sizeInSamples = rnbo_floor(sizeInSamples);
        sizeInSamples = this->maximum(sizeInSamples, 2);
        allocatedSizeInSamples = (Index)(sizeInSamples);
        allocatedSizeInSamples = nextpoweroftwo(allocatedSizeInSamples);
        return {sizeInSamples, allocatedSizeInSamples};
    }
    
    void gen_01_delay_1_init() {
        auto result = this->gen_01_delay_1_calcSizeInSamples();
        this->gen_01_delay_1__maxdelay = result[0];
        Index requestedSizeInSamples = (Index)(result[1]);
        this->gen_01_delay_1_buffer->requestSize(requestedSizeInSamples, 1);
        this->gen_01_delay_1_wrap = requestedSizeInSamples - 1;
    }
    
    void gen_01_delay_1_clear() {
        this->gen_01_delay_1_buffer->setZero();
    }
    
    void gen_01_delay_1_reset() {
        auto result = this->gen_01_delay_1_calcSizeInSamples();
        this->gen_01_delay_1__maxdelay = result[0];
        Index allocatedSizeInSamples = (Index)(result[1]);
        this->gen_01_delay_1_buffer->setSize(allocatedSizeInSamples);
        updateDataRef(this, this->gen_01_delay_1_buffer);
        this->gen_01_delay_1_wrap = this->gen_01_delay_1_buffer->getSize() - 1;
        this->gen_01_delay_1_clear();
    
        if (this->gen_01_delay_1_reader >= this->gen_01_delay_1__maxdelay || this->gen_01_delay_1_writer >= this->gen_01_delay_1__maxdelay) {
            this->gen_01_delay_1_reader = 0;
            this->gen_01_delay_1_writer = 0;
        }
    }
    
    void gen_01_delay_1_dspsetup() {
        this->gen_01_delay_1_reset();
    }
    
    number gen_01_delay_1_evaluateSizeExpr(number samplerate, number vectorsize) {
        RNBO_UNUSED(vectorsize);
        RNBO_UNUSED(samplerate);
        return this->samplerate();
    }
    
    number gen_01_delay_1_size() {
        return this->gen_01_delay_1__maxdelay;
    }
    
    number gen_01_mtof_0_next(number midivalue, number tuning) {
        RNBO_UNUSED(tuning);
    
        if (midivalue == this->gen_01_mtof_0_lastInValue && 440 == this->gen_01_mtof_0_lastTuning)
            return this->gen_01_mtof_0_lastOutValue;
    
        this->gen_01_mtof_0_lastInValue = midivalue;
        this->gen_01_mtof_0_lastTuning = 440;
        number result = 0;
    
        {
            result = rnbo_exp(.057762265 * (midivalue - 69.0));
        }
    
        this->gen_01_mtof_0_lastOutValue = 440 * result;
        return this->gen_01_mtof_0_lastOutValue;
    }
    
    void gen_01_mtof_0_reset() {
        this->gen_01_mtof_0_lastInValue = 0;
        this->gen_01_mtof_0_lastOutValue = 0;
        this->gen_01_mtof_0_lastTuning = 0;
    }
    
    number gen_01_dcblock_4_next(number x, number gain) {
        RNBO_UNUSED(gain);
        number y = x - this->gen_01_dcblock_4_xm1 + this->gen_01_dcblock_4_ym1 * 0.9997;
        this->gen_01_dcblock_4_xm1 = x;
        this->gen_01_dcblock_4_ym1 = y;
        return y;
    }
    
    void gen_01_dcblock_4_reset() {
        this->gen_01_dcblock_4_xm1 = 0;
        this->gen_01_dcblock_4_ym1 = 0;
    }
    
    void gen_01_dcblock_4_dspsetup() {
        this->gen_01_dcblock_4_reset();
    }
    
    void gen_01_dspsetup(bool force) {
        if ((bool)(this->gen_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->gen_01_setupDone = true;
        this->gen_01_delay_1_dspsetup();
        this->gen_01_dcblock_4_dspsetup();
    }
    
    void lores_01_reset() {
        this->lores_01_y_1 = 0;
        this->lores_01_y_2 = 0;
        this->lores_01_last_resonance = 0;
        this->lores_01_last_frequency = 0;
        this->lores_01_last_res_calc = 0;
        this->lores_01_last_freq_calc = 0;
    }
    
    void lores_01_dspsetup(bool force) {
        if ((bool)(this->lores_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->lores_01_reset();
        this->lores_01_setupDone = true;
    }
    
    number numbertilde_01_smooth_d_next(number x) {
        number temp = (number)(x - this->numbertilde_01_smooth_d_prev);
        this->numbertilde_01_smooth_d_prev = x;
        return temp;
    }
    
    void numbertilde_01_smooth_d_dspsetup() {
        this->numbertilde_01_smooth_d_reset();
    }
    
    void numbertilde_01_smooth_d_reset() {
        this->numbertilde_01_smooth_d_prev = 0;
    }
    
    number numbertilde_01_smooth_next(number x, number up, number down) {
        if (this->numbertilde_01_smooth_d_next(x) != 0.) {
            if (x > this->numbertilde_01_smooth_prev) {
                number _up = up;
    
                if (_up < 1)
                    _up = 1;
    
                this->numbertilde_01_smooth_index = _up;
                this->numbertilde_01_smooth_increment = (x - this->numbertilde_01_smooth_prev) / _up;
            } else if (x < this->numbertilde_01_smooth_prev) {
                number _down = down;
    
                if (_down < 1)
                    _down = 1;
    
                this->numbertilde_01_smooth_index = _down;
                this->numbertilde_01_smooth_increment = (x - this->numbertilde_01_smooth_prev) / _down;
            }
        }
    
        if (this->numbertilde_01_smooth_index > 0) {
            this->numbertilde_01_smooth_prev += this->numbertilde_01_smooth_increment;
            this->numbertilde_01_smooth_index -= 1;
        } else {
            this->numbertilde_01_smooth_prev = x;
        }
    
        return this->numbertilde_01_smooth_prev;
    }
    
    void numbertilde_01_smooth_reset() {
        this->numbertilde_01_smooth_prev = 0;
        this->numbertilde_01_smooth_index = 0;
        this->numbertilde_01_smooth_increment = 0;
        this->numbertilde_01_smooth_d_reset();
    }
    
    void numbertilde_01_init() {
        this->numbertilde_01_currentMode = 1;
        this->getEngine()->sendNumMessage(TAG("setup"), TAG("poly/number~_obj-30"), 1, this->_currentTime);
    }
    
    void numbertilde_01_dspsetup(bool force) {
        if ((bool)(this->numbertilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        this->numbertilde_01_currentIntervalInSamples = this->mstosamps(100);
        this->numbertilde_01_currentInterval = this->numbertilde_01_currentIntervalInSamples;
        this->numbertilde_01_rampInSamples = this->mstosamps(this->numbertilde_01_ramp);
        this->numbertilde_01_setupDone = true;
        this->numbertilde_01_smooth_d_dspsetup();
    }
    
    void scopetilde_01_updateEffectiveCount() {
        number effectiveCount = 256 * 1 + 256 * 0;
        this->scopetilde_01_effectiveCount = this->maximum(effectiveCount, 256);
    }
    
    void scopetilde_01_dspsetup(bool force) {
        if ((bool)(this->scopetilde_01_setupDone) && (bool)(!(bool)(force)))
            return;
    
        {
            this->scopetilde_01_mode = 1;
        }
    
        this->getEngine()->sendListMessage(
            TAG("setup"),
            TAG("poly/scope~_obj-28"),
            {1, 1, this->samplerate(), 0, 1, 0, 0, 128, this->scopetilde_01_mode},
            this->_currentTime
        );;
    
        this->scopetilde_01_updateEffectiveCount();
        this->scopetilde_01_setupDone = true;
    }
    
    void param_01_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_01_value;
    }
    
    void param_01_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_01_value_set(preset["value"]);
    }
    
    void param_02_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_02_value;
    }
    
    void param_02_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_02_value_set(preset["value"]);
    }
    
    void param_03_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_03_value;
    }
    
    void param_03_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_03_value_set(preset["value"]);
    }
    
    void param_04_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_04_value;
    }
    
    void param_04_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_04_value_set(preset["value"]);
    }
    
    void param_05_getPresetValue(PatcherStateInterface& preset) {
        preset["value"] = this->param_05_value;
    }
    
    void param_05_setPresetValue(PatcherStateInterface& preset) {
        if ((bool)(stateIsEmpty(preset)))
            return;
    
        this->param_05_value_set(preset["value"]);
    }
    
    void midiouthelper_sendMidi(number v) {
        this->midiouthelper_midiout_set(v);
    }
    
    bool stackprotect_check() {
        this->stackprotect_count++;
    
        if (this->stackprotect_count > 128) {
            console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
            return true;
        }
    
        return false;
    }
    
    void updateTime(MillisecondTime time) {
        this->_currentTime = time;
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));
    
        if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
            this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;
    
        if (this->sampleOffsetIntoNextAudioBuffer < 0)
            this->sampleOffsetIntoNextAudioBuffer = 0;
    }
    
    void assign_defaults()
    {
        pack_01_in1 = 1;
        pack_01_in2 = 0;
        pack_01_in3 = -0.5;
        pack_01_in4 = 0;
        pack_01_in5 = 0;
        pack_01_in6 = 0.5;
        expr_01_in1 = 0;
        expr_01_in2 = 128;
        expr_01_out1 = 0;
        expr_02_in1 = 0;
        expr_02_in2 = 0;
        expr_02_out1 = 0;
        mtof_01_base = 440;
        stripnote_01_velocity = 0;
        notein_01_channel = 0;
        curve_01_time = 1000;
        curve_01_curve = 0.5;
        dspexpr_01_in1 = 0;
        dspexpr_01_in2 = -1;
        dspexpr_01_in3 = 1;
        gen_01_in1 = 0;
        gen_01_feedback = 0.99;
        gen_01_note = 0;
        gen_01_dampen = 0.25;
        gen_01_a = 0;
        gen_01_b = 0;
        lores_01_cutoff = 200;
        lores_01_resonance = 0.2;
        lores_01_resonance_setter(lores_01_resonance);
        floatnum_01_input_number = 0;
        floatnum_01_value = 0;
        numbertilde_01_input_number = 0;
        numbertilde_01_ramp = 0;
        param_01_value = 2000;
        param_02_value = 0.003;
        param_03_value = 0.99;
        param_04_value = 1;
        param_05_value = 0;
        _currentTime = 0;
        audioProcessSampleCount = 0;
        sampleOffsetIntoNextAudioBuffer = 0;
        zeroBuffer = nullptr;
        dummyBuffer = nullptr;
        signals[0] = nullptr;
        signals[1] = nullptr;
        signals[2] = nullptr;
        signals[3] = nullptr;
        didAllocateSignals = 0;
        vs = 0;
        maxvs = 0;
        sr = 44100;
        invsr = 0.00002267573696;
        mtof_01_innerMtoF_lastInValue = 0;
        mtof_01_innerMtoF_lastOutValue = 0;
        mtof_01_innerMtoF_lastTuning = 0;
        mtof_01_innerScala_internal = true;
        mtof_01_innerScala_lastValid = false;
        mtof_01_innerScala_lastNote = 0;
        mtof_01_innerScala_lastFreq = 0;
        mtof_01_innerScala_sclEntryCount = 0;
        mtof_01_innerScala_sclOctaveMul = 1;
        mtof_01_innerScala_kbmValid = { 0, 0, 0, 60, 69, 440 };
        mtof_01_innerScala_kbmMid = 60;
        mtof_01_innerScala_kbmRefNum = 69;
        mtof_01_innerScala_kbmRefFreq = 440;
        mtof_01_innerScala_kbmSize = 0;
        mtof_01_innerScala_kbmMin = 0;
        mtof_01_innerScala_kbmMax = 0;
        mtof_01_innerScala_kbmOctaveDegree = 12;
        mtof_01_innerScala_kbmMapSize = 0;
        mtof_01_innerScala_refFreq = 261.63;
        notein_01_status = 0;
        notein_01_byte1 = -1;
        notein_01_inchan = 0;
        curve_01_currentValue = 0.3;
        curve_01_samplecount = 0;
        gen_01_history_6_value = 0;
        gen_01_history_5_value = 0;
        gen_01_history_4_value = 0;
        gen_01_history_3_value = 0;
        gen_01_history_2_value = 0;
        gen_01_delay_1__maxdelay = 0;
        gen_01_delay_1_sizemode = 0;
        gen_01_delay_1_wrap = 0;
        gen_01_delay_1_reader = 0;
        gen_01_delay_1_writer = 0;
        gen_01_mtof_0_lastInValue = 0;
        gen_01_mtof_0_lastOutValue = 0;
        gen_01_mtof_0_lastTuning = 0;
        gen_01_dcblock_4_xm1 = 0;
        gen_01_dcblock_4_ym1 = 0;
        gen_01_setupDone = false;
        lores_01_setupDone = false;
        floatnum_01_stored = 1;
        numbertilde_01_currentInterval = 0;
        numbertilde_01_currentIntervalInSamples = 0;
        numbertilde_01_lastValue = 0;
        numbertilde_01_outValue = 0;
        numbertilde_01_rampInSamples = 0;
        numbertilde_01_currentMode = 0;
        numbertilde_01_smooth_d_prev = 0;
        numbertilde_01_smooth_prev = 0;
        numbertilde_01_smooth_index = 0;
        numbertilde_01_smooth_increment = 0;
        numbertilde_01_setupDone = false;
        scopetilde_01_lastValue = 0;
        scopetilde_01_effectiveCount = 256;
        scopetilde_01_xsign = 1;
        scopetilde_01_ysign = 1;
        scopetilde_01_mode = 0;
        scopetilde_01_setupDone = false;
        param_01_lastValue = 0;
        param_02_lastValue = 0;
        param_03_lastValue = 0;
        param_04_lastValue = 0;
        param_05_lastValue = 0;
        stackprotect_count = 0;
        _voiceIndex = 0;
        _noteNumber = 0;
        isMuted = 1;
        parameterOffset = 0;
    }
    
    // member variables
    
        number pack_01_in1;
        number pack_01_in2;
        number pack_01_in3;
        number pack_01_in4;
        number pack_01_in5;
        number pack_01_in6;
        number expr_01_in1;
        number expr_01_in2;
        number expr_01_out1;
        number expr_02_in1;
        number expr_02_in2;
        number expr_02_out1;
        list mtof_01_midivalue;
        list mtof_01_scale;
        list mtof_01_map;
        number mtof_01_base;
        number stripnote_01_velocity;
        number notein_01_channel;
        list curve_01_dest;
        number curve_01_time;
        number curve_01_curve;
        number dspexpr_01_in1;
        number dspexpr_01_in2;
        number dspexpr_01_in3;
        number gen_01_in1;
        number gen_01_feedback;
        number gen_01_note;
        number gen_01_dampen;
        number gen_01_a;
        number gen_01_b;
        number lores_01_cutoff;
        number lores_01_resonance;
        number floatnum_01_input_number;
        number floatnum_01_value;
        number numbertilde_01_input_number;
        number numbertilde_01_ramp;
        number param_01_value;
        number param_02_value;
        number param_03_value;
        number param_04_value;
        number param_05_value;
        MillisecondTime _currentTime;
        SampleIndex audioProcessSampleCount;
        SampleIndex sampleOffsetIntoNextAudioBuffer;
        signal zeroBuffer;
        signal dummyBuffer;
        SampleValue * signals[4];
        bool didAllocateSignals;
        Index vs;
        Index maxvs;
        number sr;
        number invsr;
        number mtof_01_innerMtoF_lastInValue;
        number mtof_01_innerMtoF_lastOutValue;
        number mtof_01_innerMtoF_lastTuning;
        Float64BufferRef mtof_01_innerMtoF_buffer;
        bool mtof_01_innerScala_internal;
        const Index mtof_01_innerScala_KBM_MAP_OFFSET = 7;
        bool mtof_01_innerScala_lastValid;
        number mtof_01_innerScala_lastNote;
        number mtof_01_innerScala_lastFreq;
        int mtof_01_innerScala_sclEntryCount;
        number mtof_01_innerScala_sclOctaveMul;
        list mtof_01_innerScala_sclExpMul;
        list mtof_01_innerScala_kbmValid;
        int mtof_01_innerScala_kbmMid;
        int mtof_01_innerScala_kbmRefNum;
        number mtof_01_innerScala_kbmRefFreq;
        int mtof_01_innerScala_kbmSize;
        int mtof_01_innerScala_kbmMin;
        int mtof_01_innerScala_kbmMax;
        int mtof_01_innerScala_kbmOctaveDegree;
        Index mtof_01_innerScala_kbmMapSize;
        number mtof_01_innerScala_refFreq;
        int notein_01_status;
        int notein_01_byte1;
        int notein_01_inchan;
        list curve_01_activeRamps;
        number curve_01_currentValue;
        number curve_01_samplecount;
        number curve_01_startvalue;
        number gen_01_history_6_value;
        number gen_01_history_5_value;
        number gen_01_history_4_value;
        number gen_01_history_3_value;
        number gen_01_history_2_value;
        Float64BufferRef gen_01_delay_1_buffer;
        Index gen_01_delay_1__maxdelay;
        Int gen_01_delay_1_sizemode;
        Index gen_01_delay_1_wrap;
        Int gen_01_delay_1_reader;
        Int gen_01_delay_1_writer;
        number gen_01_mtof_0_lastInValue;
        number gen_01_mtof_0_lastOutValue;
        number gen_01_mtof_0_lastTuning;
        Float64BufferRef gen_01_mtof_0_buffer;
        number gen_01_dcblock_4_xm1;
        number gen_01_dcblock_4_ym1;
        bool gen_01_setupDone;
        number lores_01_b_0;
        number lores_01_a_1;
        number lores_01_a_2;
        number lores_01_x_0;
        number lores_01_y_1;
        number lores_01_y_2;
        number lores_01_last_resonance;
        number lores_01_last_frequency;
        number lores_01_last_res_calc;
        number lores_01_last_freq_calc;
        bool lores_01_setupDone;
        number floatnum_01_stored;
        SampleIndex numbertilde_01_currentInterval;
        SampleIndex numbertilde_01_currentIntervalInSamples;
        number numbertilde_01_lastValue;
        number numbertilde_01_outValue;
        number numbertilde_01_rampInSamples;
        Int numbertilde_01_currentMode;
        number numbertilde_01_smooth_d_prev;
        number numbertilde_01_smooth_prev;
        number numbertilde_01_smooth_index;
        number numbertilde_01_smooth_increment;
        bool numbertilde_01_setupDone;
        number scopetilde_01_lastValue;
        number scopetilde_01_effectiveCount;
        number scopetilde_01_xmonitorvalue;
        number scopetilde_01_ymonitorvalue;
        list scopetilde_01_monitorbuffer;
        number scopetilde_01_xsign;
        number scopetilde_01_ysign;
        Int scopetilde_01_mode;
        bool scopetilde_01_setupDone;
        number param_01_lastValue;
        number param_02_lastValue;
        number param_03_lastValue;
        number param_04_lastValue;
        number param_05_lastValue;
        number stackprotect_count;
        DataRef gen_01_delay_1_bufferobj;
        Index _voiceIndex;
        Int _noteNumber;
        Index isMuted;
        ParameterIndex parameterOffset;
    
};

rnbomatic()
{
}

~rnbomatic()
{
    for (int i = 0; i < 8; i++) {
        delete poly[i];
    }
}

rnbomatic* getTopLevelPatcher() {
    return this;
}

void cancelClockEvents()
{
}

template <typename T> void listquicksort(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    if (l < h) {
        Int p = (Int)(this->listpartition(arr, sortindices, l, h, ascending));
        this->listquicksort(arr, sortindices, l, p - 1, ascending);
        this->listquicksort(arr, sortindices, p + 1, h, ascending);
    }
}

template <typename T> Int listpartition(T& arr, T& sortindices, Int l, Int h, bool ascending) {
    number x = arr[(Index)h];
    Int i = (Int)(l - 1);

    for (Int j = (Int)(l); j <= h - 1; j++) {
        bool asc = (bool)((bool)(ascending) && arr[(Index)j] <= x);
        bool desc = (bool)((bool)(!(bool)(ascending)) && arr[(Index)j] >= x);

        if ((bool)(asc) || (bool)(desc)) {
            i++;
            this->listswapelements(arr, i, j);
            this->listswapelements(sortindices, i, j);
        }
    }

    i++;
    this->listswapelements(arr, i, h);
    this->listswapelements(sortindices, i, h);
    return i;
}

template <typename T> void listswapelements(T& arr, Int a, Int b) {
    auto tmp = arr[(Index)a];
    arr[(Index)a] = arr[(Index)b];
    arr[(Index)b] = tmp;
}

MillisecondTime currenttime() {
    return this->_currentTime;
}

number mstosamps(MillisecondTime ms) {
    return ms * this->sr * 0.001;
}

number maximum(number x, number y) {
    return (x < y ? y : x);
}

number tempo() {
    return this->getTopLevelPatcher()->globaltransport_getTempo(this->currenttime());
}

number mstobeats(number ms) {
    return ms * this->tempo() * 0.008 / (number)480;
}

MillisecondTime sampstoms(number samps) {
    return samps * 1000 / this->sr;
}

Index getNumMidiInputPorts() const {
    return 1;
}

void processMidiEvent(MillisecondTime time, int port, ConstByteArray data, Index length) {
    this->updateTime(time);
    this->midiin_midihandler(data[0] & 240, (data[0] & 15) + 1, port, data, length);
}

Index getNumMidiOutputPorts() const {
    return 0;
}

void process(
    const SampleValue * const* inputs,
    Index numInputs,
    SampleValue * const* outputs,
    Index numOutputs,
    Index n
) {
    RNBO_UNUSED(numInputs);
    RNBO_UNUSED(inputs);
    this->vs = n;
    this->updateTime(this->getEngine()->getCurrentTime());
    SampleValue * out1 = (numOutputs >= 1 && outputs[0] ? outputs[0] : this->dummyBuffer);
    SampleValue * out2 = (numOutputs >= 2 && outputs[1] ? outputs[1] : this->dummyBuffer);
    this->poly_perform(out1, out2, n);
    this->stackprotect_perform(n);
    this->globaltransport_advance();
    this->audioProcessSampleCount += this->vs;
}

void prepareToProcess(number sampleRate, Index maxBlockSize, bool force) {
    if (this->maxvs < maxBlockSize || !this->didAllocateSignals) {
        this->globaltransport_tempo = resizeSignal(this->globaltransport_tempo, this->maxvs, maxBlockSize);
        this->globaltransport_state = resizeSignal(this->globaltransport_state, this->maxvs, maxBlockSize);
        this->zeroBuffer = resizeSignal(this->zeroBuffer, this->maxvs, maxBlockSize);
        this->dummyBuffer = resizeSignal(this->dummyBuffer, this->maxvs, maxBlockSize);
        this->didAllocateSignals = true;
    }

    const bool sampleRateChanged = sampleRate != this->sr;
    const bool maxvsChanged = maxBlockSize != this->maxvs;
    const bool forceDSPSetup = sampleRateChanged || maxvsChanged || force;

    if (sampleRateChanged || maxvsChanged) {
        this->vs = maxBlockSize;
        this->maxvs = maxBlockSize;
        this->sr = sampleRate;
        this->invsr = 1 / sampleRate;
    }

    this->globaltransport_dspsetup(forceDSPSetup);

    for (Index i = 0; i < 8; i++) {
        this->poly[i]->prepareToProcess(sampleRate, maxBlockSize, force);
    }

    if (sampleRateChanged)
        this->onSampleRateChanged(sampleRate);
}

void setProbingTarget(MessageTag id) {
    switch (id) {
    default:
        {
        this->setProbingIndex(-1);
        break;
        }
    }
}

void setProbingIndex(ProbingIndex ) {}

Index getProbingChannels(MessageTag outletId) const {
    RNBO_UNUSED(outletId);
    return 0;
}

DataRef* getDataRef(DataRefIndex index)  {
    switch (index) {
    case 0:
        {
        return addressOf(this->RNBODefaultMtofLookupTable256);
        break;
        }
    default:
        {
        return nullptr;
        }
    }
}

DataRefIndex getNumDataRefs() const {
    return 1;
}

void fillRNBODefaultMtofLookupTable256(DataRef& ref) {
    Float64BufferRef buffer;
    buffer = new Float64Buffer(ref);
    number bufsize = buffer->getSize();

    for (Index i = 0; i < bufsize; i++) {
        number midivalue = -256. + (number)512. / (bufsize - 1) * i;
        buffer[i] = rnbo_exp(.057762265 * (midivalue - 69.0));
    }
}

void fillDataRef(DataRefIndex index, DataRef& ref) {
    switch (index) {
    case 0:
        {
        this->fillRNBODefaultMtofLookupTable256(ref);
        break;
        }
    }
}

void processDataViewUpdate(DataRefIndex index, MillisecondTime time) {
    for (Index i = 0; i < 8; i++) {
        this->poly[i]->processDataViewUpdate(index, time);
    }
}

void initialize() {
    this->RNBODefaultMtofLookupTable256 = initDataRef("RNBODefaultMtofLookupTable256", true, nullptr, "buffer~");
    this->assign_defaults();
    this->setState();
    this->RNBODefaultMtofLookupTable256->setIndex(0);
    this->initializeObjects();
    this->allocateDataRefs();
    this->startup();
}

Index getIsMuted()  {
    return this->isMuted;
}

void setIsMuted(Index v)  {
    this->isMuted = v;
}

Index getPatcherSerial() const {
    return 0;
}

void getState(PatcherStateInterface& ) {}

void setState() {
    for (Index i = 0; i < 8; i++) {
        this->poly[(Index)i] = new RNBOSubpatcher_126();
        this->poly[(Index)i]->setEngineAndPatcher(this->getEngine(), this);
        this->poly[(Index)i]->initialize();
        this->poly[(Index)i]->setParameterOffset(this->getParameterOffset(this->poly[0]));
        this->poly[(Index)i]->setVoiceIndex(i + 1);
    }
}

void getPreset(PatcherStateInterface& preset) {
    preset["__presetid"] = "rnbo";
    this->param_06_getPresetValue(getSubState(preset, "cutOff"));
    this->param_07_getPresetValue(getSubState(preset, "dampen"));
    this->param_08_getPresetValue(getSubState(preset, "feedback"));
    this->param_09_getPresetValue(getSubState(preset, "dispersion"));
    this->param_10_getPresetValue(getSubState(preset, "tuning"));

    for (Index i = 0; i < 8; i++)
        this->poly[i]->getPreset(getSubStateAt(getSubState(preset, "__sps"), "poly", i));
}

void setPreset(MillisecondTime time, PatcherStateInterface& preset) {
    this->updateTime(time);
    this->param_06_setPresetValue(getSubState(preset, "cutOff"));
    this->param_07_setPresetValue(getSubState(preset, "dampen"));
    this->param_08_setPresetValue(getSubState(preset, "feedback"));
    this->param_09_setPresetValue(getSubState(preset, "dispersion"));
    this->param_10_setPresetValue(getSubState(preset, "tuning"));
}

void processTempoEvent(MillisecondTime time, Tempo tempo) {
    this->updateTime(time);

    if (this->globaltransport_setTempo(this->_currentTime, tempo, false)) {
        for (Index i = 0; i < 8; i++) {
            this->poly[i]->processTempoEvent(time, tempo);
        }
    }
}

void processTransportEvent(MillisecondTime time, TransportState state) {
    this->updateTime(time);

    if (this->globaltransport_setState(this->_currentTime, state, false)) {
        for (Index i = 0; i < 8; i++) {
            this->poly[i]->processTransportEvent(time, state);
        }
    }
}

void processBeatTimeEvent(MillisecondTime time, BeatTime beattime) {
    this->updateTime(time);

    if (this->globaltransport_setBeatTime(this->_currentTime, beattime, false)) {
        for (Index i = 0; i < 8; i++) {
            this->poly[i]->processBeatTimeEvent(time, beattime);
        }
    }
}

void onSampleRateChanged(double ) {}

void processTimeSignatureEvent(MillisecondTime time, int numerator, int denominator) {
    this->updateTime(time);

    if (this->globaltransport_setTimeSignature(this->_currentTime, numerator, denominator, false)) {
        for (Index i = 0; i < 8; i++) {
            this->poly[i]->processTimeSignatureEvent(time, numerator, denominator);
        }
    }
}

void setParameterValue(ParameterIndex index, ParameterValue v, MillisecondTime time) {
    this->updateTime(time);

    switch (index) {
    case 0:
        {
        this->param_06_value_set(v);
        break;
        }
    case 1:
        {
        this->param_07_value_set(v);
        break;
        }
    case 2:
        {
        this->param_08_value_set(v);
        break;
        }
    case 3:
        {
        this->param_09_value_set(v);
        break;
        }
    case 4:
        {
        this->param_10_value_set(v);
        break;
        }
    default:
        {
        index -= 5;

        if (index < this->poly[0]->getNumParameters())
            this->poly[0]->setPolyParameterValue((PatcherInterface**)this->poly, index, v, time);

        break;
        }
    }
}

void processParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValue(index, value, time);
}

void processNormalizedParameterEvent(ParameterIndex index, ParameterValue value, MillisecondTime time) {
    this->setParameterValueNormalized(index, value, time);
}

ParameterValue getParameterValue(ParameterIndex index)  {
    switch (index) {
    case 0:
        {
        return this->param_06_value;
        }
    case 1:
        {
        return this->param_07_value;
        }
    case 2:
        {
        return this->param_08_value;
        }
    case 3:
        {
        return this->param_09_value;
        }
    case 4:
        {
        return this->param_10_value;
        }
    default:
        {
        index -= 5;

        if (index < this->poly[0]->getNumParameters())
            return this->poly[0]->getPolyParameterValue((PatcherInterface**)this->poly, index);

        return 0;
        }
    }
}

ParameterIndex getNumSignalInParameters() const {
    return 0;
}

ParameterIndex getNumSignalOutParameters() const {
    return 0;
}

ParameterIndex getNumParameters() const {
    return 5 + this->poly[0]->getNumParameters();
}

ConstCharPointer getParameterName(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "cutOff";
        }
    case 1:
        {
        return "dampen";
        }
    case 2:
        {
        return "feedback";
        }
    case 3:
        {
        return "dispersion";
        }
    case 4:
        {
        return "tuning";
        }
    default:
        {
        index -= 5;

        if (index < this->poly[0]->getNumParameters()) {
            {
                return this->poly[0]->getParameterName(index);
            }
        }

        return "bogus";
        }
    }
}

ConstCharPointer getParameterId(ParameterIndex index) const {
    switch (index) {
    case 0:
        {
        return "cutOff";
        }
    case 1:
        {
        return "dampen";
        }
    case 2:
        {
        return "feedback";
        }
    case 3:
        {
        return "dispersion";
        }
    case 4:
        {
        return "tuning";
        }
    default:
        {
        index -= 5;

        if (index < this->poly[0]->getNumParameters()) {
            {
                return this->poly[0]->getParameterId(index);
            }
        }

        return "bogus";
        }
    }
}

void getParameterInfo(ParameterIndex index, ParameterInfo * info) const {
    {
        switch (index) {
        case 0:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 2000;
            info->min = 200;
            info->max = 10000;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 1:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0.003;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 2:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0.99;
            info->min = 0;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 3:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 1;
            info->min = -1;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        case 4:
            {
            info->type = ParameterTypeNumber;
            info->initialValue = 0;
            info->min = -1;
            info->max = 1;
            info->exponent = 1;
            info->steps = 0;
            info->debug = false;
            info->saveable = true;
            info->transmittable = true;
            info->initialized = true;
            info->visible = true;
            info->displayName = "";
            info->unit = "";
            info->ioType = IOTypeUndefined;
            info->signalIndex = INVALID_INDEX;
            break;
            }
        default:
            {
            index -= 5;

            if (index < this->poly[0]->getNumParameters()) {
                for (Index i = 0; i < 8; i++) {
                    this->poly[i]->getParameterInfo(index, info);
                }
            }

            break;
            }
        }
    }
}

void sendParameter(ParameterIndex index, bool ignoreValue) {
    this->getEngine()->notifyParameterValueChanged(index, (ignoreValue ? 0 : this->getParameterValue(index)), ignoreValue);
}

ParameterIndex getParameterOffset(BaseInterface* subpatcher) const {
    if (subpatcher == this->poly[0])
        return 5;

    return 0;
}

ParameterValue applyStepsToNormalizedParameterValue(ParameterValue normalizedValue, int steps) const {
    if (steps == 1) {
        if (normalizedValue > 0) {
            normalizedValue = 1.;
        }
    } else {
        ParameterValue oneStep = (number)1. / (steps - 1);
        ParameterValue numberOfSteps = rnbo_fround(normalizedValue / oneStep * 1 / (number)1) * (number)1;
        normalizedValue = numberOfSteps * oneStep;
    }

    return normalizedValue;
}

ParameterValue convertToNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 1:
    case 2:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - 0) / (1 - 0);
            return normalizedValue;
        }
        }
    case 0:
        {
        {
            value = (value < 200 ? 200 : (value > 10000 ? 10000 : value));
            ParameterValue normalizedValue = (value - 200) / (10000 - 200);
            return normalizedValue;
        }
        }
    case 3:
    case 4:
        {
        {
            value = (value < -1 ? -1 : (value > 1 ? 1 : value));
            ParameterValue normalizedValue = (value - -1) / (1 - -1);
            return normalizedValue;
        }
        }
    default:
        {
        index -= 5;

        if (index < this->poly[0]->getNumParameters()) {
            {
                return this->poly[0]->convertToNormalizedParameterValue(index, value);
            }
        }

        return value;
        }
    }
}

ParameterValue convertFromNormalizedParameterValue(ParameterIndex index, ParameterValue value) const {
    value = (value < 0 ? 0 : (value > 1 ? 1 : value));

    switch (index) {
    case 1:
    case 2:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 0 + value * (1 - 0);
            }
        }
        }
    case 0:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return 200 + value * (10000 - 200);
            }
        }
        }
    case 3:
    case 4:
        {
        {
            value = (value < 0 ? 0 : (value > 1 ? 1 : value));

            {
                return -1 + value * (1 - -1);
            }
        }
        }
    default:
        {
        index -= 5;

        if (index < this->poly[0]->getNumParameters()) {
            {
                return this->poly[0]->convertFromNormalizedParameterValue(index, value);
            }
        }

        return value;
        }
    }
}

ParameterValue constrainParameterValue(ParameterIndex index, ParameterValue value) const {
    switch (index) {
    case 0:
        {
        return this->param_06_value_constrain(value);
        }
    case 1:
        {
        return this->param_07_value_constrain(value);
        }
    case 2:
        {
        return this->param_08_value_constrain(value);
        }
    case 3:
        {
        return this->param_09_value_constrain(value);
        }
    case 4:
        {
        return this->param_10_value_constrain(value);
        }
    default:
        {
        index -= 5;

        if (index < this->poly[0]->getNumParameters()) {
            {
                return this->poly[0]->constrainParameterValue(index, value);
            }
        }

        return value;
        }
    }
}

void scheduleParamInit(ParameterIndex index, Index order) {
    this->paramInitIndices->push(index);
    this->paramInitOrder->push(order);
}

void processParamInitEvents() {
    this->listquicksort(
        this->paramInitOrder,
        this->paramInitIndices,
        0,
        (int)(this->paramInitOrder->length - 1),
        true
    );

    for (Index i = 0; i < this->paramInitOrder->length; i++) {
        this->getEngine()->scheduleParameterChange(
            this->paramInitIndices[i],
            this->getParameterValue(this->paramInitIndices[i]),
            0
        );
    }
}

void processClockEvent(MillisecondTime , ClockId , bool , ParameterValue ) {}

void processOutletAtCurrentTime(EngineLink* , OutletIndex , ParameterValue ) {}

void processOutletEvent(
    EngineLink* sender,
    OutletIndex index,
    ParameterValue value,
    MillisecondTime time
) {
    this->updateTime(time);
    this->processOutletAtCurrentTime(sender, index, value);
}

void processNumMessage(MessageTag tag, MessageTag objectId, MillisecondTime time, number payload) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);

    for (Index i = 0; i < 8; i++) {
        this->poly[i]->processNumMessage(tag, objectId, time, payload);
    }
}

void processListMessage(
    MessageTag tag,
    MessageTag objectId,
    MillisecondTime time,
    const list& payload
) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);

    for (Index i = 0; i < 8; i++) {
        this->poly[i]->processListMessage(tag, objectId, time, payload);
    }
}

void processBangMessage(MessageTag tag, MessageTag objectId, MillisecondTime time) {
    RNBO_UNUSED(objectId);
    this->updateTime(time);

    for (Index i = 0; i < 8; i++) {
        this->poly[i]->processBangMessage(tag, objectId, time);
    }
}

MessageTagInfo resolveTag(MessageTag tag) const {
    switch (tag) {

    }

    auto subpatchResult_0 = this->poly[0]->resolveTag(tag);

    if (subpatchResult_0)
        return subpatchResult_0;

    return "";
}

MessageIndex getNumMessages() const {
    return 0;
}

const MessageInfo& getMessageInfo(MessageIndex index) const {
    switch (index) {

    }

    return NullMessageInfo;
}

protected:

void param_06_value_set(number v) {
    v = this->param_06_value_constrain(v);
    this->param_06_value = v;
    this->sendParameter(0, false);

    if (this->param_06_value != this->param_06_lastValue) {
        this->getEngine()->presetTouched();
        this->param_06_lastValue = this->param_06_value;
    }

    this->poly_cutOff_set(v);
}

void param_07_value_set(number v) {
    v = this->param_07_value_constrain(v);
    this->param_07_value = v;
    this->sendParameter(1, false);

    if (this->param_07_value != this->param_07_lastValue) {
        this->getEngine()->presetTouched();
        this->param_07_lastValue = this->param_07_value;
    }

    this->poly_dampen_set(v);
}

void param_08_value_set(number v) {
    v = this->param_08_value_constrain(v);
    this->param_08_value = v;
    this->sendParameter(2, false);

    if (this->param_08_value != this->param_08_lastValue) {
        this->getEngine()->presetTouched();
        this->param_08_lastValue = this->param_08_value;
    }

    this->poly_feedback_set(v);
}

void param_09_value_set(number v) {
    v = this->param_09_value_constrain(v);
    this->param_09_value = v;
    this->sendParameter(3, false);

    if (this->param_09_value != this->param_09_lastValue) {
        this->getEngine()->presetTouched();
        this->param_09_lastValue = this->param_09_value;
    }

    this->poly_dispersion_set(v);
}

void param_10_value_set(number v) {
    v = this->param_10_value_constrain(v);
    this->param_10_value = v;
    this->sendParameter(4, false);

    if (this->param_10_value != this->param_10_lastValue) {
        this->getEngine()->presetTouched();
        this->param_10_lastValue = this->param_10_value;
    }

    this->poly_tuning_set(v);
}

number msToSamps(MillisecondTime ms, number sampleRate) {
    return ms * sampleRate * 0.001;
}

MillisecondTime sampsToMs(SampleIndex samps) {
    return samps * (this->invsr * 1000);
}

Index getMaxBlockSize() const {
    return this->maxvs;
}

number getSampleRate() const {
    return this->sr;
}

bool hasFixedVectorSize() const {
    return false;
}

Index getNumInputChannels() const {
    return 0;
}

Index getNumOutputChannels() const {
    return 2;
}

void allocateDataRefs() {
    for (Index i = 0; i < 8; i++) {
        this->poly[i]->allocateDataRefs();
    }

    if (this->RNBODefaultMtofLookupTable256->hasRequestedSize()) {
        if (this->RNBODefaultMtofLookupTable256->wantsFill())
            this->fillRNBODefaultMtofLookupTable256(this->RNBODefaultMtofLookupTable256);

        this->getEngine()->sendDataRefUpdated(0);
    }
}

void initializeObjects() {
    this->midinotecontroller_01_init();

    for (Index i = 0; i < 8; i++) {
        this->poly[i]->initializeObjects();
    }
}

void sendOutlet(OutletIndex index, ParameterValue value) {
    this->getEngine()->sendOutlet(this, index, value);
}

void startup() {
    this->updateTime(this->getEngine()->getCurrentTime());

    for (Index i = 0; i < 8; i++) {
        this->poly[i]->startup();
    }

    {
        this->scheduleParamInit(0, 0);
    }

    {
        this->scheduleParamInit(1, 0);
    }

    {
        this->scheduleParamInit(2, 0);
    }

    {
        this->scheduleParamInit(3, 0);
    }

    {
        this->scheduleParamInit(4, 0);
    }

    this->processParamInitEvents();
}

static number param_06_value_constrain(number v) {
    v = (v > 10000 ? 10000 : (v < 200 ? 200 : v));
    return v;
}

void poly_cutOff_set(number v) {
    for (number i = 0; i < 8; i++) {
        if (i + 1 == this->poly_target || 0 == this->poly_target) {
            this->poly[(Index)i]->setParameterValue(0, v, this->_currentTime);
        }
    }
}

static number param_07_value_constrain(number v) {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    return v;
}

void poly_dampen_set(number v) {
    for (number i = 0; i < 8; i++) {
        if (i + 1 == this->poly_target || 0 == this->poly_target) {
            this->poly[(Index)i]->setParameterValue(1, v, this->_currentTime);
        }
    }
}

static number param_08_value_constrain(number v) {
    v = (v > 1 ? 1 : (v < 0 ? 0 : v));
    return v;
}

void poly_feedback_set(number v) {
    for (number i = 0; i < 8; i++) {
        if (i + 1 == this->poly_target || 0 == this->poly_target) {
            this->poly[(Index)i]->setParameterValue(2, v, this->_currentTime);
        }
    }
}

static number param_09_value_constrain(number v) {
    v = (v > 1 ? 1 : (v < -1 ? -1 : v));
    return v;
}

void poly_dispersion_set(number v) {
    for (number i = 0; i < 8; i++) {
        if (i + 1 == this->poly_target || 0 == this->poly_target) {
            this->poly[(Index)i]->setParameterValue(3, v, this->_currentTime);
        }
    }
}

static number param_10_value_constrain(number v) {
    v = (v > 1 ? 1 : (v < -1 ? -1 : v));
    return v;
}

void poly_tuning_set(number v) {
    for (number i = 0; i < 8; i++) {
        if (i + 1 == this->poly_target || 0 == this->poly_target) {
            this->poly[(Index)i]->setParameterValue(4, v, this->_currentTime);
        }
    }
}

void midinotecontroller_01_currenttarget_set(number v) {
    this->midinotecontroller_01_currenttarget = v;
}

void poly_target_set(number v) {
    this->poly_target = v;
    this->midinotecontroller_01_currenttarget_set(v);
}

void midinotecontroller_01_target_set(number v) {
    this->poly_target_set(v);
}

void poly_midiininternal_set(number v) {
    Index sendlen = 0;
    this->poly_currentStatus = parseMidi(this->poly_currentStatus, (int)(v), this->poly_mididata[0]);

    switch ((int)this->poly_currentStatus) {
    case MIDI_StatusByteReceived:
        {
        this->poly_mididata[0] = (uint8_t)(v);
        this->poly_mididata[1] = 0;
        break;
        }
    case MIDI_SecondByteReceived:
    case MIDI_ProgramChange:
    case MIDI_ChannelPressure:
        {
        this->poly_mididata[1] = (uint8_t)(v);

        if (this->poly_currentStatus == MIDI_ProgramChange || this->poly_currentStatus == MIDI_ChannelPressure) {
            sendlen = 2;
        }

        break;
        }
    case MIDI_NoteOff:
    case MIDI_NoteOn:
    case MIDI_Aftertouch:
    case MIDI_CC:
    case MIDI_PitchBend:
    default:
        {
        this->poly_mididata[2] = (uint8_t)(v);
        sendlen = 3;
        break;
        }
    }

    if (sendlen > 0) {
        number i;

        if (this->poly_target > 0 && this->poly_target <= 8) {
            i = this->poly_target - 1;
            this->poly[(Index)i]->processMidiEvent(_currentTime, 0, this->poly_mididata, sendlen);
        } else if (this->poly_target == 0) {
            for (i = 0; i < 8; i++) {
                this->poly[(Index)i]->processMidiEvent(_currentTime, 0, this->poly_mididata, sendlen);
            }
        }
    }
}

void midinotecontroller_01_midiout_set(number v) {
    this->poly_midiininternal_set(v);
}

void poly_noteNumber_set(number v) {
    if (this->poly_target > 0) {
        this->poly[(Index)(this->poly_target - 1)]->setNoteNumber((int)(v));
    }
}

void midinotecontroller_01_noteNumber_set(number v) {
    this->poly_noteNumber_set(v);
}

void midinotecontroller_01_voicestatus_set(const list& v) {
    if (v[1] == 1) {
        number currentTarget = this->midinotecontroller_01_currenttarget;
        this->midinotecontroller_01_target_set(v[0]);
        this->midinotecontroller_01_noteNumber_set(0);
        this->midinotecontroller_01_target_set(currentTarget);
    }
}

void poly_voicestatus_set(const list& v) {
    this->midinotecontroller_01_voicestatus_set(v);
}

void poly_activevoices_set(number ) {}

void poly_mute_set(const list& v) {
    Index voiceNumber = (Index)(v[0]);
    Index muteState = (Index)(v[1]);

    if (voiceNumber == 0) {
        for (Index i = 0; i < 8; i++) {
            this->poly[(Index)i]->setIsMuted(muteState);
        }
    } else {
        Index subpatcherIndex = voiceNumber - 1;

        if (subpatcherIndex >= 0 && subpatcherIndex < 8) {
            this->poly[(Index)subpatcherIndex]->setIsMuted(muteState);
        }
    }

    list tmp = {v[0], v[1]};
    this->poly_voicestatus_set(tmp);
    this->poly_activevoices_set(this->poly_calcActiveVoices());
}

void midinotecontroller_01_mute_set(const list& v) {
    this->poly_mute_set(v);
}

void midinotecontroller_01_midiin_set(number v) {
    this->midinotecontroller_01_midiin = v;
    int val = (int)(v);

    this->midinotecontroller_01_currentStatus = parseMidi(
        this->midinotecontroller_01_currentStatus,
        (int)(v),
        this->midinotecontroller_01_status
    );

    switch ((int)this->midinotecontroller_01_currentStatus) {
    case MIDI_StatusByteReceived:
        {
        {
            this->midinotecontroller_01_status = val;
            this->midinotecontroller_01_byte1 = -1;
            break;
        }
        }
    case MIDI_SecondByteReceived:
        {
        this->midinotecontroller_01_byte1 = val;
        break;
        }
    case MIDI_NoteOn:
        {
        {
            bool sendnoteoff = true;
            number target = 1;
            number oldest = this->midinotecontroller_01_voice_lastontime[0];
            number target_state = this->midinotecontroller_01_voice_state[0];

            for (Index i = 0; i < 8; i++) {
                number candidate_state = this->midinotecontroller_01_voice_state[(Index)i];

                if (this->midinotecontroller_01_voice_notenumber[(Index)i] == this->midinotecontroller_01_byte1 && candidate_state == MIDI_NoteState_On) {
                    sendnoteoff = false;
                    target = i + 1;
                    break;
                }

                if (i > 0) {
                    if (candidate_state != MIDI_NoteState_On || target_state == MIDI_NoteState_On) {
                        number candidate_ontime = this->midinotecontroller_01_voice_lastontime[(Index)i];

                        if (candidate_ontime < oldest || (target_state == MIDI_NoteState_On && candidate_state != MIDI_NoteState_On)) {
                            target = i + 1;
                            oldest = candidate_ontime;
                            target_state = candidate_state;
                        }
                    }
                }
            }

            if ((bool)(sendnoteoff))
                this->midinotecontroller_01_sendnoteoff((int)(target));

            int i = (int)(target - 1);
            this->midinotecontroller_01_voice_state[(Index)i] = MIDI_NoteState_On;
            this->midinotecontroller_01_voice_lastontime[(Index)i] = this->currenttime();
            this->midinotecontroller_01_voice_notenumber[(Index)i] = this->midinotecontroller_01_byte1;
            this->midinotecontroller_01_voice_channel[(Index)i] = (BinOpInt)((BinOpInt)this->midinotecontroller_01_status & (BinOpInt)0x0F);

            for (Index j = 0; j < 128; j++) {
                if (this->midinotecontroller_01_notesdown[(Index)j] == 0) {
                    this->midinotecontroller_01_notesdown[(Index)j] = this->midinotecontroller_01_voice_notenumber[(Index)i];
                    break;
                }
            }

            this->midinotecontroller_01_note_lastvelocity[(Index)this->midinotecontroller_01_voice_notenumber[(Index)i]] = val;
            this->midinotecontroller_01_sendpitchbend((int)(i));
            this->midinotecontroller_01_sendpressure((int)(i));
            this->midinotecontroller_01_sendtimbre((int)(i));
            this->midinotecontroller_01_muteval[0] = target;
            this->midinotecontroller_01_muteval[1] = 0;
            this->midinotecontroller_01_mute_set(this->midinotecontroller_01_muteval);
            number currentTarget = this->midinotecontroller_01_currenttarget;
            this->midinotecontroller_01_target_set(target);
            this->midinotecontroller_01_noteNumber_set(this->midinotecontroller_01_voice_notenumber[(Index)i]);

            this->midinotecontroller_01_midiout_set(
                (BinOpInt)((BinOpInt)MIDI_NoteOnMask | (BinOpInt)this->midinotecontroller_01_voice_channel[(Index)i])
            );

            this->midinotecontroller_01_midiout_set(this->midinotecontroller_01_voice_notenumber[(Index)i]);
            this->midinotecontroller_01_midiout_set(val);
            this->midinotecontroller_01_target_set(currentTarget);
            break;
        }
        }
    case MIDI_NoteOff:
        {
        {
            number target = 0;
            number notenumber = this->midinotecontroller_01_byte1;
            number channel = (BinOpInt)((BinOpInt)this->midinotecontroller_01_status & (BinOpInt)0x0F);

            for (Index i = 0; i < 8; i++) {
                if (this->midinotecontroller_01_voice_notenumber[(Index)i] == notenumber && this->midinotecontroller_01_voice_channel[(Index)i] == channel && this->midinotecontroller_01_voice_state[(Index)i] == MIDI_NoteState_On) {
                    target = i + 1;
                    break;
                }
            }

            if (target > 0) {
                int i = (int)(target - 1);
                Index j = (Index)(channel);
                bool ignoresustainchannel = true;

                if ((bool)(this->midinotecontroller_01_channel_sustain[((bool)(ignoresustainchannel) ? 0 : j)])) {
                    this->midinotecontroller_01_voice_state[(Index)i] = MIDI_NoteState_Sustained;
                } else {
                    number currentTarget = this->midinotecontroller_01_currenttarget;
                    this->midinotecontroller_01_target_set(target);
                    this->midinotecontroller_01_midiout_set(this->midinotecontroller_01_status);
                    this->midinotecontroller_01_midiout_set(this->midinotecontroller_01_byte1);
                    this->midinotecontroller_01_midiout_set(v);
                    this->midinotecontroller_01_target_set(currentTarget);

                    if (this->midinotecontroller_01_currentStatus == MIDI_NoteOff) {
                        this->midinotecontroller_01_voice_state[(Index)i] = MIDI_NoteState_Off;
                    }
                }
            } else
                {}

            bool found = false;

            for (Index i = 0; i < 128; i++) {
                if (this->midinotecontroller_01_notesdown[(Index)i] == 0) {
                    break;
                } else if (this->midinotecontroller_01_notesdown[(Index)i] == notenumber) {
                    found = true;
                }

                if ((bool)(found)) {
                    this->midinotecontroller_01_notesdown[(Index)i] = this->midinotecontroller_01_notesdown[(Index)(i + 1)];
                }
            }

            break;
        }
        }
    case MIDI_Aftertouch:
        {
        {
            number currentTarget = this->midinotecontroller_01_currenttarget;
            this->midinotecontroller_01_target_set(0);
            this->midinotecontroller_01_midiout_set(this->midinotecontroller_01_status);
            this->midinotecontroller_01_midiout_set(this->midinotecontroller_01_byte1);
            this->midinotecontroller_01_midiout_set(v);
            this->midinotecontroller_01_target_set(currentTarget);
            break;
        }
        }
    case MIDI_CC:
        {
        {
            bool sendToAllVoices = true;

            switch ((int)this->midinotecontroller_01_byte1) {
            case MIDI_CC_Sustain:
                {
                {
                    bool pedaldown = (bool)((val >= 64 ? true : false));
                    number channel = (BinOpInt)((BinOpInt)this->midinotecontroller_01_status & (BinOpInt)0x0F);
                    Index j = (Index)(channel);
                    bool ignoresustainchannel = true;
                    this->midinotecontroller_01_channel_sustain[((bool)(ignoresustainchannel) ? 0 : j)] = pedaldown;

                    if ((bool)(!(bool)(pedaldown))) {
                        for (Index i = 0; i < 8; i++) {
                            if (((bool)(ignoresustainchannel) || this->midinotecontroller_01_voice_channel[(Index)i] == channel) && this->midinotecontroller_01_voice_state[(Index)i] == MIDI_NoteState_Sustained) {
                                number currentTarget = this->midinotecontroller_01_currenttarget;
                                this->midinotecontroller_01_target_set(i + 1);
                                this->midinotecontroller_01_midiout_set((BinOpInt)((BinOpInt)MIDI_NoteOffMask | (BinOpInt)j));
                                this->midinotecontroller_01_midiout_set(this->midinotecontroller_01_voice_notenumber[(Index)i]);
                                this->midinotecontroller_01_midiout_set(64);
                                this->midinotecontroller_01_target_set(currentTarget);
                                this->midinotecontroller_01_voice_state[(Index)i] = MIDI_NoteState_Off;
                            }
                        }
                    }

                    break;
                }
                }
            case MIDI_CC_TimbreMSB:
                {
                {
                    number channel = (BinOpInt)((BinOpInt)this->midinotecontroller_01_status & (BinOpInt)0x0F);
                    int k = (int)(v);
                    number timbre = (BinOpInt)(((BinOpInt)((BinOpInt)k & (BinOpInt)0x7F)) << imod_nocast((UBinOpInt)7, 32));
                    this->midinotecontroller_01_channel_timbre[(Index)((BinOpInt)this->midinotecontroller_01_status & (BinOpInt)0x0F)] = timbre;

                    for (Index i = 0; i < 8; i++) {
                        if (this->midinotecontroller_01_voice_channel[(Index)i] == channel && this->midinotecontroller_01_voice_state[(Index)i] != MIDI_NoteState_Off) {
                            this->midinotecontroller_01_sendtimbre(i);
                        }
                    }

                    sendToAllVoices = false;
                    break;
                }
                }
            case MIDI_CC_TimbreLSB:
                {
                {
                    break;
                }
                }
            case MIDI_CC_AllNotesOff:
                {
                {
                    this->midinotecontroller_01_sendallnotesoff();
                    break;
                }
                }
            }

            if ((bool)(sendToAllVoices)) {
                number currentTarget = this->midinotecontroller_01_currenttarget;
                this->midinotecontroller_01_target_set(0);
                this->midinotecontroller_01_midiout_set(this->midinotecontroller_01_status);
                this->midinotecontroller_01_midiout_set(this->midinotecontroller_01_byte1);
                this->midinotecontroller_01_midiout_set(v);
                this->midinotecontroller_01_target_set(currentTarget);
            }

            break;
        }
        }
    case MIDI_ProgramChange:
        {
        {
            number currentTarget = this->midinotecontroller_01_currenttarget;
            this->midinotecontroller_01_target_set(0);
            this->midinotecontroller_01_midiout_set(this->midinotecontroller_01_status);
            this->midinotecontroller_01_midiout_set(this->midinotecontroller_01_byte1);
            this->midinotecontroller_01_target_set(currentTarget);
            break;
        }
        }
    case MIDI_ChannelPressure:
        {
        {
            number channel = (BinOpInt)((BinOpInt)this->midinotecontroller_01_status & (BinOpInt)0x0F);

            for (number i = 0; i < 8; i++) {
                if (this->midinotecontroller_01_voice_state[(Index)i] != MIDI_NoteState_Off && this->midinotecontroller_01_voice_channel[(Index)i] == channel) {
                    int k = (int)(channel);
                    this->midinotecontroller_01_channel_pressure[(Index)k] = v;
                    this->midinotecontroller_01_sendpressure(i);
                }
            }

            break;
        }
        }
    case MIDI_PitchBend:
        {
        {
            number bendamount = (BinOpInt)((BinOpInt)this->midinotecontroller_01_byte1 | (BinOpInt)((BinOpInt)val << imod_nocast((UBinOpInt)7, 32)));
            int channel = (int)((BinOpInt)((BinOpInt)this->midinotecontroller_01_status & (BinOpInt)0x0F));
            this->midinotecontroller_01_channel_pitchbend[(Index)channel] = bendamount;

            for (Index i = 0; i < 8; i++) {
                if (this->midinotecontroller_01_voice_state[(Index)i] != MIDI_NoteState_Off && this->midinotecontroller_01_voice_channel[(Index)i] == channel) {
                    this->midinotecontroller_01_sendpitchbend(i);
                }
            }

            break;
        }
        }
    }
}

void poly_midiin_set(number v) {
    this->poly_midiin = v;
    this->midinotecontroller_01_midiin_set(v);
}

void midiin_midiout_set(number v) {
    this->poly_midiin_set(v);
}

void midiin_midihandler(int status, int channel, int port, ConstByteArray data, Index length) {
    RNBO_UNUSED(port);
    RNBO_UNUSED(channel);
    RNBO_UNUSED(status);
    Index i;

    for (i = 0; i < length; i++) {
        this->midiin_midiout_set(data[i]);
    }
}

void poly_perform(SampleValue * out1, SampleValue * out2, Index n) {
    SampleArray<2> outs = {out1, out2};

    for (number chan = 0; chan < 2; chan++)
        zeroSignal(outs[(Index)chan], n);

    for (Index i = 0; i < 8; i++)
        this->poly[(Index)i]->process(nullptr, 0, outs, 2, n);
}

void stackprotect_perform(Index n) {
    RNBO_UNUSED(n);
    auto __stackprotect_count = this->stackprotect_count;
    __stackprotect_count = 0;
    this->stackprotect_count = __stackprotect_count;
}

void param_06_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_06_value;
}

void param_06_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_06_value_set(preset["value"]);
}

void param_07_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_07_value;
}

void param_07_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_07_value_set(preset["value"]);
}

void param_08_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_08_value;
}

void param_08_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_08_value_set(preset["value"]);
}

void param_09_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_09_value;
}

void param_09_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_09_value_set(preset["value"]);
}

void param_10_getPresetValue(PatcherStateInterface& preset) {
    preset["value"] = this->param_10_value;
}

void param_10_setPresetValue(PatcherStateInterface& preset) {
    if ((bool)(stateIsEmpty(preset)))
        return;

    this->param_10_value_set(preset["value"]);
}

number poly_calcActiveVoices() {
    {
        number activeVoices = 0;

        for (Index i = 0; i < 8; i++) {
            if ((bool)(!(bool)(this->poly[(Index)i]->getIsMuted())))
                activeVoices++;
        }

        return activeVoices;
    }
}

void midinotecontroller_01_init() {
    for (Index i = 0; i < 16; i++) {
        this->midinotecontroller_01_channel_pitchbend[(Index)i] = 0x2000;
    }

    for (Index i = 0; i < 8; i++) {
        this->midinotecontroller_01_voice_lastontime[(Index)i] = -1;
    }
}

void midinotecontroller_01_sendnoteoff(int target) {
    int i = (int)(target - 1);

    if (this->midinotecontroller_01_voice_state[(Index)i] != MIDI_NoteState_Off) {
        number currentTarget = this->midinotecontroller_01_currenttarget;
        this->midinotecontroller_01_target_set(target);

        this->midinotecontroller_01_midiout_set(
            (BinOpInt)((BinOpInt)MIDI_NoteOffMask | (BinOpInt)this->midinotecontroller_01_voice_channel[(Index)i])
        );

        this->midinotecontroller_01_midiout_set(this->midinotecontroller_01_voice_notenumber[(Index)i]);
        this->midinotecontroller_01_midiout_set(64);
        this->midinotecontroller_01_voice_state[(Index)i] = MIDI_NoteState_Off;
        this->midinotecontroller_01_target_set(currentTarget);
    }
}

void midinotecontroller_01_sendpitchbend(int v) {
    if (v >= 0 && v < 8) {
        number currentTarget = this->midinotecontroller_01_currenttarget;
        this->midinotecontroller_01_target_set(v + 1);
        int totalbendamount = (int)(this->midinotecontroller_01_channel_pitchbend[(Index)this->midinotecontroller_01_voice_channel[(Index)v]]);

        this->midinotecontroller_01_midiout_set(
            (BinOpInt)((BinOpInt)MIDI_PitchBendMask | (BinOpInt)this->midinotecontroller_01_voice_channel[(Index)v])
        );

        this->midinotecontroller_01_midiout_set((BinOpInt)((BinOpInt)totalbendamount & (BinOpInt)0x7F));

        this->midinotecontroller_01_midiout_set(
            (BinOpInt)((BinOpInt)((BinOpInt)totalbendamount >> imod_nocast((UBinOpInt)imod_nocast((UBinOpInt)7, 32), 32)) & (BinOpInt)0x7F)
        );

        this->midinotecontroller_01_target_set(currentTarget);
    }
}

void midinotecontroller_01_sendpressure(int v) {
    number currentTarget = this->midinotecontroller_01_currenttarget;
    this->midinotecontroller_01_target_set(v + 1);

    this->midinotecontroller_01_midiout_set(
        (BinOpInt)((BinOpInt)MIDI_ChannelPressureMask | (BinOpInt)this->midinotecontroller_01_voice_channel[(Index)v])
    );

    this->midinotecontroller_01_midiout_set(
        this->midinotecontroller_01_channel_pressure[(Index)this->midinotecontroller_01_voice_channel[(Index)v]]
    );

    this->midinotecontroller_01_target_set(currentTarget);
}

void midinotecontroller_01_sendtimbre(int v) {
    number currentTarget = this->midinotecontroller_01_currenttarget;
    this->midinotecontroller_01_target_set(v + 1);

    this->midinotecontroller_01_midiout_set(
        (BinOpInt)((BinOpInt)MIDI_CCMask | (BinOpInt)this->midinotecontroller_01_voice_channel[(Index)v])
    );

    this->midinotecontroller_01_midiout_set(MIDI_CC_TimbreLSB);

    this->midinotecontroller_01_midiout_set(
        (BinOpInt)((BinOpInt)this->midinotecontroller_01_channel_timbre[(Index)this->midinotecontroller_01_voice_channel[(Index)v]] & (BinOpInt)0x7F)
    );

    this->midinotecontroller_01_midiout_set(
        (BinOpInt)((BinOpInt)MIDI_CCMask | (BinOpInt)this->midinotecontroller_01_voice_channel[(Index)v])
    );

    this->midinotecontroller_01_midiout_set(MIDI_CC_TimbreMSB);

    this->midinotecontroller_01_midiout_set(
        (BinOpInt)((BinOpInt)((BinOpInt)this->midinotecontroller_01_channel_timbre[(Index)this->midinotecontroller_01_voice_channel[(Index)v]] >> imod_nocast((UBinOpInt)7, 32)) & (BinOpInt)0x7F)
    );

    this->midinotecontroller_01_target_set(currentTarget);
}

void midinotecontroller_01_sendallnotesoff() {
    for (Index i = 1; i <= 8; i++) {
        this->midinotecontroller_01_sendnoteoff(i);
    }
}

Index globaltransport_getSampleOffset(MillisecondTime time) {
    return this->mstosamps(this->maximum(0, time - this->getEngine()->getCurrentTime()));
}

number globaltransport_getTempoAtSample(SampleIndex sampleOffset) {
    return (sampleOffset >= 0 && sampleOffset < this->vs ? this->globaltransport_tempo[(Index)sampleOffset] : this->globaltransport_lastTempo);
}

number globaltransport_getStateAtSample(SampleIndex sampleOffset) {
    return (sampleOffset >= 0 && sampleOffset < this->vs ? this->globaltransport_state[(Index)sampleOffset] : this->globaltransport_lastState);
}

number globaltransport_getState(MillisecondTime time) {
    return this->globaltransport_getStateAtSample(this->globaltransport_getSampleOffset(time));
}

number globaltransport_getBeatTime(MillisecondTime time) {
    number i = 2;

    while (i < this->globaltransport_beatTimeChanges->length && this->globaltransport_beatTimeChanges[(Index)(i + 1)] <= time) {
        i += 2;
    }

    i -= 2;
    number beatTimeBase = this->globaltransport_beatTimeChanges[(Index)i];

    if (this->globaltransport_getState(time) == 0)
        return beatTimeBase;

    number beatTimeBaseMsTime = this->globaltransport_beatTimeChanges[(Index)(i + 1)];
    number diff = time - beatTimeBaseMsTime;
    return beatTimeBase + this->mstobeats(diff);
}

bool globaltransport_setTempo(MillisecondTime time, number tempo, bool notify) {
    if ((bool)(notify)) {
        this->processTempoEvent(time, tempo);
        this->globaltransport_notify = true;
    } else {
        Index offset = (Index)(this->globaltransport_getSampleOffset(time));

        if (this->globaltransport_getTempoAtSample(offset) != tempo) {
            this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTime(time));
            this->globaltransport_beatTimeChanges->push(time);
            fillSignal(this->globaltransport_tempo, this->vs, tempo, offset);
            this->globaltransport_lastTempo = tempo;
            this->globaltransport_tempoNeedsReset = true;
            return true;
        }
    }

    return false;
}

number globaltransport_getTempo(MillisecondTime time) {
    return this->globaltransport_getTempoAtSample(this->globaltransport_getSampleOffset(time));
}

bool globaltransport_setState(MillisecondTime time, number state, bool notify) {
    if ((bool)(notify)) {
        this->processTransportEvent(time, TransportState(state));
        this->globaltransport_notify = true;
    } else {
        Index offset = (Index)(this->globaltransport_getSampleOffset(time));

        if (this->globaltransport_getStateAtSample(offset) != state) {
            fillSignal(this->globaltransport_state, this->vs, state, offset);
            this->globaltransport_lastState = TransportState(state);
            this->globaltransport_stateNeedsReset = true;

            if (state == 0) {
                this->globaltransport_beatTimeChanges->push(this->globaltransport_getBeatTime(time));
                this->globaltransport_beatTimeChanges->push(time);
            }

            return true;
        }
    }

    return false;
}

bool globaltransport_setBeatTime(MillisecondTime time, number beattime, bool notify) {
    if ((bool)(notify)) {
        this->processBeatTimeEvent(time, beattime);
        this->globaltransport_notify = true;
        return false;
    } else {
        bool beatTimeHasChanged = false;
        float oldBeatTime = (float)(this->globaltransport_getBeatTime(time));
        float newBeatTime = (float)(beattime);

        if (oldBeatTime != newBeatTime) {
            beatTimeHasChanged = true;
        }

        this->globaltransport_beatTimeChanges->push(beattime);
        this->globaltransport_beatTimeChanges->push(time);
        return beatTimeHasChanged;
    }
}

number globaltransport_getBeatTimeAtSample(SampleIndex sampleOffset) {
    auto msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getBeatTime(this->getEngine()->getCurrentTime() + msOffset);
}

array<number, 2> globaltransport_getTimeSignature(MillisecondTime time) {
    number i = 3;

    while (i < this->globaltransport_timeSignatureChanges->length && this->globaltransport_timeSignatureChanges[(Index)(i + 2)] <= time) {
        i += 3;
    }

    i -= 3;

    return {
        this->globaltransport_timeSignatureChanges[(Index)i],
        this->globaltransport_timeSignatureChanges[(Index)(i + 1)]
    };
}

array<number, 2> globaltransport_getTimeSignatureAtSample(SampleIndex sampleOffset) {
    auto msOffset = this->sampstoms(sampleOffset);
    return this->globaltransport_getTimeSignature(this->getEngine()->getCurrentTime() + msOffset);
}

bool globaltransport_setTimeSignature(MillisecondTime time, number numerator, number denominator, bool notify) {
    if ((bool)(notify)) {
        this->processTimeSignatureEvent(time, (int)(numerator), (int)(denominator));
        this->globaltransport_notify = true;
    } else {
        array<number, 2> currentSig = this->globaltransport_getTimeSignature(time);

        if (currentSig[0] != numerator || currentSig[1] != denominator) {
            this->globaltransport_timeSignatureChanges->push(numerator);
            this->globaltransport_timeSignatureChanges->push(denominator);
            this->globaltransport_timeSignatureChanges->push(time);
            return true;
        }
    }

    return false;
}

void globaltransport_advance() {
    if ((bool)(this->globaltransport_tempoNeedsReset)) {
        fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
        this->globaltransport_tempoNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTempoEvent(this->globaltransport_lastTempo);
        }
    }

    if ((bool)(this->globaltransport_stateNeedsReset)) {
        fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
        this->globaltransport_stateNeedsReset = false;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTransportEvent(TransportState(this->globaltransport_lastState));
        }
    }

    if (this->globaltransport_beatTimeChanges->length > 2) {
        this->globaltransport_beatTimeChanges[0] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 2)];
        this->globaltransport_beatTimeChanges[1] = this->globaltransport_beatTimeChanges[(Index)(this->globaltransport_beatTimeChanges->length - 1)];
        this->globaltransport_beatTimeChanges->length = 2;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendBeatTimeEvent(this->globaltransport_beatTimeChanges[0]);
        }
    }

    if (this->globaltransport_timeSignatureChanges->length > 3) {
        this->globaltransport_timeSignatureChanges[0] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 3)];
        this->globaltransport_timeSignatureChanges[1] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 2)];
        this->globaltransport_timeSignatureChanges[2] = this->globaltransport_timeSignatureChanges[(Index)(this->globaltransport_timeSignatureChanges->length - 1)];
        this->globaltransport_timeSignatureChanges->length = 3;

        if ((bool)(this->globaltransport_notify)) {
            this->getEngine()->sendTimeSignatureEvent(
                (int)(this->globaltransport_timeSignatureChanges[0]),
                (int)(this->globaltransport_timeSignatureChanges[1])
            );
        }
    }

    this->globaltransport_notify = false;
}

void globaltransport_dspsetup(bool force) {
    if ((bool)(this->globaltransport_setupDone) && (bool)(!(bool)(force)))
        return;

    fillSignal(this->globaltransport_tempo, this->vs, this->globaltransport_lastTempo);
    this->globaltransport_tempoNeedsReset = false;
    fillSignal(this->globaltransport_state, this->vs, this->globaltransport_lastState);
    this->globaltransport_stateNeedsReset = false;
    this->globaltransport_setupDone = true;
}

bool stackprotect_check() {
    this->stackprotect_count++;

    if (this->stackprotect_count > 128) {
        console->log("STACK OVERFLOW DETECTED - stopped processing branch !");
        return true;
    }

    return false;
}

void updateTime(MillisecondTime time) {
    this->_currentTime = time;
    this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(rnbo_fround(this->msToSamps(time - this->getEngine()->getCurrentTime(), this->sr)));

    if (this->sampleOffsetIntoNextAudioBuffer >= (SampleIndex)(this->vs))
        this->sampleOffsetIntoNextAudioBuffer = (SampleIndex)(this->vs) - 1;

    if (this->sampleOffsetIntoNextAudioBuffer < 0)
        this->sampleOffsetIntoNextAudioBuffer = 0;
}

void assign_defaults()
{
    midiin_port = 0;
    param_06_value = 2000;
    param_07_value = 0.003;
    param_08_value = 0.99;
    param_09_value = 1;
    param_10_value = 0;
    poly_target = 0;
    poly_midiin = 0;
    midinotecontroller_01_currenttarget = 0;
    midinotecontroller_01_midiin = 0;
    _currentTime = 0;
    audioProcessSampleCount = 0;
    sampleOffsetIntoNextAudioBuffer = 0;
    zeroBuffer = nullptr;
    dummyBuffer = nullptr;
    didAllocateSignals = 0;
    vs = 0;
    maxvs = 0;
    sr = 44100;
    invsr = 0.00002267573696;
    param_06_lastValue = 0;
    param_07_lastValue = 0;
    param_08_lastValue = 0;
    param_09_lastValue = 0;
    param_10_lastValue = 0;
    poly_currentStatus = -1;
    poly_mididata[0] = 0;
    poly_mididata[1] = 0;
    poly_mididata[2] = 0;
    midinotecontroller_01_currentStatus = -1;
    midinotecontroller_01_status = -1;
    midinotecontroller_01_byte1 = -1;
    midinotecontroller_01_zone_masterchannel = 1;
    midinotecontroller_01_zone_numnotechannels = 15;
    midinotecontroller_01_zone_masterpitchbendrange = 2;
    midinotecontroller_01_zone_pernotepitchbendrange = 48;
    midinotecontroller_01_muteval = { 0, 0 };
    globaltransport_tempo = nullptr;
    globaltransport_tempoNeedsReset = false;
    globaltransport_lastTempo = 120;
    globaltransport_state = nullptr;
    globaltransport_stateNeedsReset = false;
    globaltransport_lastState = 0;
    globaltransport_beatTimeChanges = { 0, 0 };
    globaltransport_timeSignatureChanges = { 4, 4, 0 };
    globaltransport_notify = false;
    globaltransport_setupDone = false;
    stackprotect_count = 0;
    _voiceIndex = 0;
    _noteNumber = 0;
    isMuted = 1;
}

// member variables

    number midiin_port;
    number param_06_value;
    number param_07_value;
    number param_08_value;
    number param_09_value;
    number param_10_value;
    number poly_target;
    number poly_midiin;
    number midinotecontroller_01_currenttarget;
    number midinotecontroller_01_midiin;
    MillisecondTime _currentTime;
    SampleIndex audioProcessSampleCount;
    SampleIndex sampleOffsetIntoNextAudioBuffer;
    signal zeroBuffer;
    signal dummyBuffer;
    bool didAllocateSignals;
    Index vs;
    Index maxvs;
    number sr;
    number invsr;
    number param_06_lastValue;
    number param_07_lastValue;
    number param_08_lastValue;
    number param_09_lastValue;
    number param_10_lastValue;
    int poly_currentStatus;
    uint8_t poly_mididata[3];
    int midinotecontroller_01_currentStatus;
    int midinotecontroller_01_status;
    int midinotecontroller_01_byte1;
    int midinotecontroller_01_zone_masterchannel;
    int midinotecontroller_01_zone_numnotechannels;
    int midinotecontroller_01_zone_masterpitchbendrange;
    int midinotecontroller_01_zone_pernotepitchbendrange;
    number midinotecontroller_01_channel_pitchbend[16] = { };
    number midinotecontroller_01_channel_pressure[16] = { };
    number midinotecontroller_01_channel_timbre[16] = { };
    Int midinotecontroller_01_channel_sustain[16] = { };
    number midinotecontroller_01_voice_lastontime[8] = { };
    number midinotecontroller_01_voice_notenumber[8] = { };
    number midinotecontroller_01_voice_channel[8] = { };
    number midinotecontroller_01_voice_state[8] = { };
    number midinotecontroller_01_notesdown[129] = { };
    number midinotecontroller_01_note_lastvelocity[128] = { };
    list midinotecontroller_01_muteval;
    signal globaltransport_tempo;
    bool globaltransport_tempoNeedsReset;
    number globaltransport_lastTempo;
    signal globaltransport_state;
    bool globaltransport_stateNeedsReset;
    number globaltransport_lastState;
    list globaltransport_beatTimeChanges;
    list globaltransport_timeSignatureChanges;
    bool globaltransport_notify;
    bool globaltransport_setupDone;
    number stackprotect_count;
    DataRef RNBODefaultMtofLookupTable256;
    Index _voiceIndex;
    Int _noteNumber;
    Index isMuted;
    indexlist paramInitIndices;
    indexlist paramInitOrder;
    RNBOSubpatcher_126* poly[8];

};

PatcherInterface* creaternbomatic()
{
    return new rnbomatic();
}

#ifndef RNBO_NO_PATCHERFACTORY

extern "C" PatcherFactoryFunctionPtr GetPatcherFactoryFunction(PlatformInterface* platformInterface)
#else

extern "C" PatcherFactoryFunctionPtr rnbomaticFactoryFunction(PlatformInterface* platformInterface)
#endif

{
    Platform::set(platformInterface);
    return creaternbomatic;
}

} // end RNBO namespace

