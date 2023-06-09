

#define LOG_TAG "gles1"
//#define OPENGL_DEBUG_PRINTOUT
//#define CHECK_GL_ERRORS
#define LOG_CALL_NAME 0


#include <string.h>
#include "gles1_opcodes.h"
#include "gles1_dec.h"

#include "ProtocolUtils.h"
#include "ChecksumCalculatorThreadInfo.h"
#include <stdio.h>
#ifdef OPENGL_DEBUG_PRINTOUT
#  define DEBUG(...) do { if (emugl_cxt_logger) { emugl_cxt_logger(__VA_ARGS__); } } while(0)
#else
#  define DEBUG(...)  ((void)0)
#endif

#ifdef CHECK_GL_ERRORS
#  define SET_LASTCALL(name)  sprintf(lastCall, #name)
#else
#  define SET_LASTCALL(name)  ((void)0)
#endif
using namespace emugl;

size_t gles1_decoder_context_t::decode(void *buf, size_t len, IOStream *stream,
                                       ChecksumCalculator *checksumCalc) {
    if (len < 8) return 0;
#ifdef CHECK_GL_ERRORS
    char lastCall[256] = {0};
#endif
    unsigned char *ptr = (unsigned char *) buf;
    const unsigned char *const end = (const unsigned char *) buf + len;
    const size_t checksumSize = checksumCalc->checksumByteSize();
    const bool useChecksum = checksumSize > 0;
    while (end - ptr >= 8) {
        uint32_t opcode = *(uint32_t *) ptr;
        int32_t packetLen = *(int32_t *) (ptr + 4);
        if (end - ptr < packetLen) return ptr - (unsigned char *) buf;
        switch (opcode) {
            case OP_glAlphaFunc: {
                GLenum var_func = Unpack<GLenum, uint32_t>(ptr + 8);
                GLclampf var_ref = Unpack<GLclampf, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glAlphaFunc: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glAlphaFunc\n", err);
#endif
                DEBUG("gles1(%p): glAlphaFunc(0x%08x %f )\n", stream, var_func, var_ref);
                this->glAlphaFunc(var_func, var_ref);
                SET_LASTCALL("glAlphaFunc");
                break;
            }
            case OP_glClearColor: {
                GLclampf var_red = Unpack<GLclampf, uint32_t>(ptr + 8);
                GLclampf var_green = Unpack<GLclampf, uint32_t>(ptr + 8 + 4);
                GLclampf var_blue = Unpack<GLclampf, uint32_t>(ptr + 8 + 4 + 4);
                GLclampf var_alpha = Unpack<GLclampf, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClearColor: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClearColor\n", err);
#endif
                DEBUG("gles1(%p): glClearColor(%f %f %f %f )\n", stream, var_red, var_green,
                      var_blue, var_alpha);
                this->glClearColor(var_red, var_green, var_blue, var_alpha);
                SET_LASTCALL("glClearColor");
                break;
            }
            case OP_glClearDepthf: {
                GLclampf var_depth = Unpack<GLclampf, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClearDepthf: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClearDepthf\n", err);
#endif
                DEBUG("gles1(%p): glClearDepthf(%f )\n", stream, var_depth);
                this->glClearDepthf(var_depth);
                SET_LASTCALL("glClearDepthf");
                break;
            }
            case OP_glClipPlanef: {
                GLenum var_plane = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_equation __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_equation(ptr + 8 + 4 + 4, size_equation);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_equation,
                                                             ptr + 8 + 4 + 4 + size_equation,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClipPlanef: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClipPlanef\n", err);
#endif
                DEBUG("gles1(%p): glClipPlanef(0x%08x %p(%u) )\n", stream, var_plane,
                      (const GLfloat *) (inptr_equation.get()), size_equation);
                this->glClipPlanef(var_plane, (const GLfloat *) (inptr_equation.get()));
                SET_LASTCALL("glClipPlanef");
                break;
            }
            case OP_glColor4f: {
                GLfloat var_red = Unpack<GLfloat, uint32_t>(ptr + 8);
                GLfloat var_green = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                GLfloat var_blue = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                GLfloat var_alpha = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glColor4f: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glColor4f\n", err);
#endif
                DEBUG("gles1(%p): glColor4f(%f %f %f %f )\n", stream, var_red, var_green, var_blue,
                      var_alpha);
                this->glColor4f(var_red, var_green, var_blue, var_alpha);
                SET_LASTCALL("glColor4f");
                break;
            }
            case OP_glDepthRangef: {
                GLclampf var_zNear = Unpack<GLclampf, uint32_t>(ptr + 8);
                GLclampf var_zFar = Unpack<GLclampf, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDepthRangef: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDepthRangef\n", err);
#endif
                DEBUG("gles1(%p): glDepthRangef(%f %f )\n", stream, var_zNear, var_zFar);
                this->glDepthRangef(var_zNear, var_zFar);
                SET_LASTCALL("glDepthRangef");
                break;
            }
            case OP_glFogf: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                GLfloat var_param = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFogf: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFogf\n", err);
#endif
                DEBUG("gles1(%p): glFogf(0x%08x %f )\n", stream, var_pname, var_param);
                this->glFogf(var_pname, var_param);
                SET_LASTCALL("glFogf");
                break;
            }
            case OP_glFogfv: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFogfv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFogfv\n", err);
#endif
                DEBUG("gles1(%p): glFogfv(0x%08x %p(%u) )\n", stream, var_pname,
                      (const GLfloat *) (inptr_params.get()), size_params);
                this->glFogfv(var_pname, (const GLfloat *) (inptr_params.get()));
                SET_LASTCALL("glFogfv");
                break;
            }
            case OP_glFrustumf: {
                GLfloat var_left = Unpack<GLfloat, uint32_t>(ptr + 8);
                GLfloat var_right = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                GLfloat var_bottom = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                GLfloat var_top = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfloat var_zNear = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLfloat var_zFar = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFrustumf: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFrustumf\n", err);
#endif
                DEBUG("gles1(%p): glFrustumf(%f %f %f %f %f %f )\n", stream, var_left, var_right,
                      var_bottom, var_top, var_zNear, var_zFar);
                this->glFrustumf(var_left, var_right, var_bottom, var_top, var_zNear, var_zFar);
                SET_LASTCALL("glFrustumf");
                break;
            }
            case OP_glGetClipPlanef: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_eqn __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetClipPlanef: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_eqn;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_eqn(&tmpBuf[0], size_eqn);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetClipPlanef\n", err);
#endif
                DEBUG("gles1(%p): glGetClipPlanef(0x%08x %p(%u) )\n", stream, var_pname,
                      (GLfloat *) (outptr_eqn.get()), size_eqn);
                this->glGetClipPlanef(var_pname, (GLfloat *) (outptr_eqn.get()));
                outptr_eqn.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetClipPlanef");
                break;
            }
            case OP_glGetFloatv: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetFloatv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetFloatv\n", err);
#endif
                DEBUG("gles1(%p): glGetFloatv(0x%08x %p(%u) )\n", stream, var_pname,
                      (GLfloat *) (outptr_params.get()), size_params);
                this->glGetFloatv(var_pname, (GLfloat *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetFloatv");
                break;
            }
            case OP_glGetLightfv: {
                GLenum var_light = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetLightfv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetLightfv\n", err);
#endif
                DEBUG("gles1(%p): glGetLightfv(0x%08x 0x%08x %p(%u) )\n", stream, var_light,
                      var_pname, (GLfloat *) (outptr_params.get()), size_params);
                this->glGetLightfv(var_light, var_pname, (GLfloat *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetLightfv");
                break;
            }
            case OP_glGetMaterialfv: {
                GLenum var_face = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetMaterialfv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetMaterialfv\n", err);
#endif
                DEBUG("gles1(%p): glGetMaterialfv(0x%08x 0x%08x %p(%u) )\n", stream, var_face,
                      var_pname, (GLfloat *) (outptr_params.get()), size_params);
                this->glGetMaterialfv(var_face, var_pname, (GLfloat *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetMaterialfv");
                break;
            }
            case OP_glGetTexEnvfv: {
                GLenum var_env = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetTexEnvfv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetTexEnvfv\n", err);
#endif
                DEBUG("gles1(%p): glGetTexEnvfv(0x%08x 0x%08x %p(%u) )\n", stream, var_env,
                      var_pname, (GLfloat *) (outptr_params.get()), size_params);
                this->glGetTexEnvfv(var_env, var_pname, (GLfloat *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetTexEnvfv");
                break;
            }
            case OP_glGetTexParameterfv: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetTexParameterfv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetTexParameterfv\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetTexParameterfv(0x%08x 0x%08x %p(%u) )\n", stream, var_target,
                      var_pname, (GLfloat *) (outptr_params.get()), size_params);
                this->glGetTexParameterfv(var_target, var_pname, (GLfloat *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetTexParameterfv");
                break;
            }
            case OP_glLightModelf: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                GLfloat var_param = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLightModelf: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLightModelf\n", err);
#endif
                DEBUG("gles1(%p): glLightModelf(0x%08x %f )\n", stream, var_pname, var_param);
                this->glLightModelf(var_pname, var_param);
                SET_LASTCALL("glLightModelf");
                break;
            }
            case OP_glLightModelfv: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLightModelfv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLightModelfv\n", err);
#endif
                DEBUG("gles1(%p): glLightModelfv(0x%08x %p(%u) )\n", stream, var_pname,
                      (const GLfloat *) (inptr_params.get()), size_params);
                this->glLightModelfv(var_pname, (const GLfloat *) (inptr_params.get()));
                SET_LASTCALL("glLightModelfv");
                break;
            }
            case OP_glLightf: {
                GLenum var_light = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfloat var_param = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLightf: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLightf\n", err);
#endif
                DEBUG("gles1(%p): glLightf(0x%08x 0x%08x %f )\n", stream, var_light, var_pname,
                      var_param);
                this->glLightf(var_light, var_pname, var_param);
                SET_LASTCALL("glLightf");
                break;
            }
            case OP_glLightfv: {
                GLenum var_light = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLightfv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLightfv\n", err);
#endif
                DEBUG("gles1(%p): glLightfv(0x%08x 0x%08x %p(%u) )\n", stream, var_light, var_pname,
                      (const GLfloat *) (inptr_params.get()), size_params);
                this->glLightfv(var_light, var_pname, (const GLfloat *) (inptr_params.get()));
                SET_LASTCALL("glLightfv");
                break;
            }
            case OP_glLineWidth: {
                GLfloat var_width = Unpack<GLfloat, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLineWidth: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLineWidth\n", err);
#endif
                DEBUG("gles1(%p): glLineWidth(%f )\n", stream, var_width);
                this->glLineWidth(var_width);
                SET_LASTCALL("glLineWidth");
                break;
            }
            case OP_glLoadMatrixf: {
                uint32_t size_m __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                InputBuffer inptr_m(ptr + 8 + 4, size_m);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + size_m,
                                                             ptr + 8 + 4 + size_m, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLoadMatrixf: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLoadMatrixf\n", err);
#endif
                DEBUG("gles1(%p): glLoadMatrixf(%p(%u) )\n", stream,
                      (const GLfloat *) (inptr_m.get()), size_m);
                this->glLoadMatrixf((const GLfloat *) (inptr_m.get()));
                SET_LASTCALL("glLoadMatrixf");
                break;
            }
            case OP_glMaterialf: {
                GLenum var_face = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfloat var_param = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMaterialf: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMaterialf\n", err);
#endif
                DEBUG("gles1(%p): glMaterialf(0x%08x 0x%08x %f )\n", stream, var_face, var_pname,
                      var_param);
                this->glMaterialf(var_face, var_pname, var_param);
                SET_LASTCALL("glMaterialf");
                break;
            }
            case OP_glMaterialfv: {
                GLenum var_face = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMaterialfv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMaterialfv\n", err);
#endif
                DEBUG("gles1(%p): glMaterialfv(0x%08x 0x%08x %p(%u) )\n", stream, var_face,
                      var_pname, (const GLfloat *) (inptr_params.get()), size_params);
                this->glMaterialfv(var_face, var_pname, (const GLfloat *) (inptr_params.get()));
                SET_LASTCALL("glMaterialfv");
                break;
            }
            case OP_glMultMatrixf: {
                uint32_t size_m __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                InputBuffer inptr_m(ptr + 8 + 4, size_m);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + size_m,
                                                             ptr + 8 + 4 + size_m, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMultMatrixf: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMultMatrixf\n", err);
#endif
                DEBUG("gles1(%p): glMultMatrixf(%p(%u) )\n", stream,
                      (const GLfloat *) (inptr_m.get()), size_m);
                this->glMultMatrixf((const GLfloat *) (inptr_m.get()));
                SET_LASTCALL("glMultMatrixf");
                break;
            }
            case OP_glMultiTexCoord4f: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLfloat var_s = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                GLfloat var_t = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                GLfloat var_r = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfloat var_q = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMultiTexCoord4f: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMultiTexCoord4f\n", err);
#endif
                DEBUG("gles1(%p): glMultiTexCoord4f(0x%08x %f %f %f %f )\n", stream, var_target,
                      var_s, var_t, var_r, var_q);
                this->glMultiTexCoord4f(var_target, var_s, var_t, var_r, var_q);
                SET_LASTCALL("glMultiTexCoord4f");
                break;
            }
            case OP_glNormal3f: {
                GLfloat var_nx = Unpack<GLfloat, uint32_t>(ptr + 8);
                GLfloat var_ny = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                GLfloat var_nz = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glNormal3f: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glNormal3f\n", err);
#endif
                DEBUG("gles1(%p): glNormal3f(%f %f %f )\n", stream, var_nx, var_ny, var_nz);
                this->glNormal3f(var_nx, var_ny, var_nz);
                SET_LASTCALL("glNormal3f");
                break;
            }
            case OP_glOrthof: {
                GLfloat var_left = Unpack<GLfloat, uint32_t>(ptr + 8);
                GLfloat var_right = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                GLfloat var_bottom = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                GLfloat var_top = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfloat var_zNear = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLfloat var_zFar = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glOrthof: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glOrthof\n", err);
#endif
                DEBUG("gles1(%p): glOrthof(%f %f %f %f %f %f )\n", stream, var_left, var_right,
                      var_bottom, var_top, var_zNear, var_zFar);
                this->glOrthof(var_left, var_right, var_bottom, var_top, var_zNear, var_zFar);
                SET_LASTCALL("glOrthof");
                break;
            }
            case OP_glPointParameterf: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                GLfloat var_param = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPointParameterf: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPointParameterf\n", err);
#endif
                DEBUG("gles1(%p): glPointParameterf(0x%08x %f )\n", stream, var_pname, var_param);
                this->glPointParameterf(var_pname, var_param);
                SET_LASTCALL("glPointParameterf");
                break;
            }
            case OP_glPointParameterfv: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPointParameterfv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPointParameterfv\n",
                            err);
#endif
                DEBUG("gles1(%p): glPointParameterfv(0x%08x %p(%u) )\n", stream, var_pname,
                      (const GLfloat *) (inptr_params.get()), size_params);
                this->glPointParameterfv(var_pname, (const GLfloat *) (inptr_params.get()));
                SET_LASTCALL("glPointParameterfv");
                break;
            }
            case OP_glPointSize: {
                GLfloat var_size = Unpack<GLfloat, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPointSize: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPointSize\n", err);
#endif
                DEBUG("gles1(%p): glPointSize(%f )\n", stream, var_size);
                this->glPointSize(var_size);
                SET_LASTCALL("glPointSize");
                break;
            }
            case OP_glPolygonOffset: {
                GLfloat var_factor = Unpack<GLfloat, uint32_t>(ptr + 8);
                GLfloat var_units = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPolygonOffset: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPolygonOffset\n", err);
#endif
                DEBUG("gles1(%p): glPolygonOffset(%f %f )\n", stream, var_factor, var_units);
                this->glPolygonOffset(var_factor, var_units);
                SET_LASTCALL("glPolygonOffset");
                break;
            }
            case OP_glRotatef: {
                GLfloat var_angle = Unpack<GLfloat, uint32_t>(ptr + 8);
                GLfloat var_x = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                GLfloat var_y = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                GLfloat var_z = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glRotatef: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glRotatef\n", err);
#endif
                DEBUG("gles1(%p): glRotatef(%f %f %f %f )\n", stream, var_angle, var_x, var_y,
                      var_z);
                this->glRotatef(var_angle, var_x, var_y, var_z);
                SET_LASTCALL("glRotatef");
                break;
            }
            case OP_glScalef: {
                GLfloat var_x = Unpack<GLfloat, uint32_t>(ptr + 8);
                GLfloat var_y = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                GLfloat var_z = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glScalef: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glScalef\n", err);
#endif
                DEBUG("gles1(%p): glScalef(%f %f %f )\n", stream, var_x, var_y, var_z);
                this->glScalef(var_x, var_y, var_z);
                SET_LASTCALL("glScalef");
                break;
            }
            case OP_glTexEnvf: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfloat var_param = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexEnvf: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexEnvf\n", err);
#endif
                DEBUG("gles1(%p): glTexEnvf(0x%08x 0x%08x %f )\n", stream, var_target, var_pname,
                      var_param);
                this->glTexEnvf(var_target, var_pname, var_param);
                SET_LASTCALL("glTexEnvf");
                break;
            }
            case OP_glTexEnvfv: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexEnvfv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexEnvfv\n", err);
#endif
                DEBUG("gles1(%p): glTexEnvfv(0x%08x 0x%08x %p(%u) )\n", stream, var_target,
                      var_pname, (const GLfloat *) (inptr_params.get()), size_params);
                this->glTexEnvfv(var_target, var_pname, (const GLfloat *) (inptr_params.get()));
                SET_LASTCALL("glTexEnvfv");
                break;
            }
            case OP_glTexParameterf: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfloat var_param = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexParameterf: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexParameterf\n", err);
#endif
                DEBUG("gles1(%p): glTexParameterf(0x%08x 0x%08x %f )\n", stream, var_target,
                      var_pname, var_param);
                this->glTexParameterf(var_target, var_pname, var_param);
                SET_LASTCALL("glTexParameterf");
                break;
            }
            case OP_glTexParameterfv: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexParameterfv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexParameterfv\n", err);
#endif
                DEBUG("gles1(%p): glTexParameterfv(0x%08x 0x%08x %p(%u) )\n", stream, var_target,
                      var_pname, (const GLfloat *) (inptr_params.get()), size_params);
                this->glTexParameterfv(var_target, var_pname,
                                       (const GLfloat *) (inptr_params.get()));
                SET_LASTCALL("glTexParameterfv");
                break;
            }
            case OP_glTranslatef: {
                GLfloat var_x = Unpack<GLfloat, uint32_t>(ptr + 8);
                GLfloat var_y = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                GLfloat var_z = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTranslatef: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTranslatef\n", err);
#endif
                DEBUG("gles1(%p): glTranslatef(%f %f %f )\n", stream, var_x, var_y, var_z);
                this->glTranslatef(var_x, var_y, var_z);
                SET_LASTCALL("glTranslatef");
                break;
            }
            case OP_glActiveTexture: {
                GLenum var_texture = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glActiveTexture: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glActiveTexture\n", err);
#endif
                DEBUG("gles1(%p): glActiveTexture(0x%08x )\n", stream, var_texture);
                this->glActiveTexture(var_texture);
                SET_LASTCALL("glActiveTexture");
                break;
            }
            case OP_glAlphaFuncx: {
                GLenum var_func = Unpack<GLenum, uint32_t>(ptr + 8);
                GLclampx var_ref = Unpack<GLclampx, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glAlphaFuncx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glAlphaFuncx\n", err);
#endif
                DEBUG("gles1(%p): glAlphaFuncx(0x%08x 0x%08x )\n", stream, var_func, var_ref);
                this->glAlphaFuncx(var_func, var_ref);
                SET_LASTCALL("glAlphaFuncx");
                break;
            }
            case OP_glBindBuffer: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLuint var_buffer = Unpack<GLuint, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glBindBuffer: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glBindBuffer\n", err);
#endif
                DEBUG("gles1(%p): glBindBuffer(0x%08x %u )\n", stream, var_target, var_buffer);
                this->glBindBuffer(var_target, var_buffer);
                SET_LASTCALL("glBindBuffer");
                break;
            }
            case OP_glBindTexture: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLuint var_texture = Unpack<GLuint, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glBindTexture: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glBindTexture\n", err);
#endif
                DEBUG("gles1(%p): glBindTexture(0x%08x %u )\n", stream, var_target, var_texture);
                this->glBindTexture(var_target, var_texture);
                SET_LASTCALL("glBindTexture");
                break;
            }
            case OP_glBlendFunc: {
                GLenum var_sfactor = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_dfactor = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glBlendFunc: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glBlendFunc\n", err);
#endif
                DEBUG("gles1(%p): glBlendFunc(0x%08x 0x%08x )\n", stream, var_sfactor, var_dfactor);
                this->glBlendFunc(var_sfactor, var_dfactor);
                SET_LASTCALL("glBlendFunc");
                break;
            }
            case OP_glBufferData: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLsizeiptr var_size = Unpack<GLsizeiptr, uint32_t>(ptr + 8 + 4);
                uint32_t size_data __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_data(ptr + 8 + 4 + 4 + 4, size_data);
                GLenum var_usage = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4 + 4 + size_data);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_data + 4,
                                                             ptr + 8 + 4 + 4 + 4 + size_data + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glBufferData: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glBufferData\n", err);
#endif
                DEBUG("gles1(%p): glBufferData(0x%08x 0x%08lx %p(%u) 0x%08x )\n", stream,
                      var_target, var_size, (const GLvoid *) (inptr_data.get()), size_data,
                      var_usage);
                this->glBufferData(var_target, var_size,
                                   size_data == 0 ? nullptr : (const GLvoid *) (inptr_data.get()),
                                   var_usage);
                SET_LASTCALL("glBufferData");
                break;
            }
            case OP_glBufferSubData: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLintptr var_offset = Unpack<GLintptr, uint32_t>(ptr + 8 + 4);
                GLsizeiptr var_size = Unpack<GLsizeiptr, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_data __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                InputBuffer inptr_data(ptr + 8 + 4 + 4 + 4 + 4, size_data);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + size_data,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + size_data,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glBufferSubData: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glBufferSubData\n", err);
#endif
                DEBUG("gles1(%p): glBufferSubData(0x%08x 0x%08lx 0x%08lx %p(%u) )\n", stream,
                      var_target, var_offset, var_size, (const GLvoid *) (inptr_data.get()),
                      size_data);
                this->glBufferSubData(var_target, var_offset, var_size, size_data == 0 ? nullptr
                                                                                       : (const GLvoid *) (inptr_data.get()));
                SET_LASTCALL("glBufferSubData");
                break;
            }
            case OP_glClear: {
                GLbitfield var_mask = Unpack<GLbitfield, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClear: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClear\n", err);
#endif
                DEBUG("gles1(%p): glClear(0x%08x )\n", stream, var_mask);
                this->glClear(var_mask);
                SET_LASTCALL("glClear");
                break;
            }
            case OP_glClearColorx: {
                GLclampx var_red = Unpack<GLclampx, uint32_t>(ptr + 8);
                GLclampx var_green = Unpack<GLclampx, uint32_t>(ptr + 8 + 4);
                GLclampx var_blue = Unpack<GLclampx, uint32_t>(ptr + 8 + 4 + 4);
                GLclampx var_alpha = Unpack<GLclampx, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClearColorx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClearColorx\n", err);
#endif
                DEBUG("gles1(%p): glClearColorx(0x%08x 0x%08x 0x%08x 0x%08x )\n", stream, var_red,
                      var_green, var_blue, var_alpha);
                this->glClearColorx(var_red, var_green, var_blue, var_alpha);
                SET_LASTCALL("glClearColorx");
                break;
            }
            case OP_glClearDepthx: {
                GLclampx var_depth = Unpack<GLclampx, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClearDepthx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClearDepthx\n", err);
#endif
                DEBUG("gles1(%p): glClearDepthx(0x%08x )\n", stream, var_depth);
                this->glClearDepthx(var_depth);
                SET_LASTCALL("glClearDepthx");
                break;
            }
            case OP_glClearStencil: {
                GLint var_s = Unpack<GLint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClearStencil: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClearStencil\n", err);
#endif
                DEBUG("gles1(%p): glClearStencil(%d )\n", stream, var_s);
                this->glClearStencil(var_s);
                SET_LASTCALL("glClearStencil");
                break;
            }
            case OP_glClientActiveTexture: {
                GLenum var_texture = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClientActiveTexture: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClientActiveTexture\n",
                            err);
#endif
                DEBUG("gles1(%p): glClientActiveTexture(0x%08x )\n", stream, var_texture);
                this->glClientActiveTexture(var_texture);
                SET_LASTCALL("glClientActiveTexture");
                break;
            }
            case OP_glColor4ub: {
                GLubyte var_red = Unpack<GLubyte, uint8_t>(ptr + 8);
                GLubyte var_green = Unpack<GLubyte, uint8_t>(ptr + 8 + 1);
                GLubyte var_blue = Unpack<GLubyte, uint8_t>(ptr + 8 + 1 + 1);
                GLubyte var_alpha = Unpack<GLubyte, uint8_t>(ptr + 8 + 1 + 1 + 1);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 1 + 1 + 1 + 1,
                                                             ptr + 8 + 1 + 1 + 1 + 1, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glColor4ub: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glColor4ub\n", err);
#endif
                DEBUG("gles1(%p): glColor4ub(0x%02x 0x%02x 0x%02x 0x%02x )\n", stream, var_red,
                      var_green, var_blue, var_alpha);
                this->glColor4ub(var_red, var_green, var_blue, var_alpha);
                SET_LASTCALL("glColor4ub");
                break;
            }
            case OP_glColor4x: {
                GLfixed var_red = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_green = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_blue = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                GLfixed var_alpha = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glColor4x: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glColor4x\n", err);
#endif
                DEBUG("gles1(%p): glColor4x(0x%08x 0x%08x 0x%08x 0x%08x )\n", stream, var_red,
                      var_green, var_blue, var_alpha);
                this->glColor4x(var_red, var_green, var_blue, var_alpha);
                SET_LASTCALL("glColor4x");
                break;
            }
            case OP_glColorMask: {
                GLboolean var_red = Unpack<GLboolean, uint8_t>(ptr + 8);
                GLboolean var_green = Unpack<GLboolean, uint8_t>(ptr + 8 + 1);
                GLboolean var_blue = Unpack<GLboolean, uint8_t>(ptr + 8 + 1 + 1);
                GLboolean var_alpha = Unpack<GLboolean, uint8_t>(ptr + 8 + 1 + 1 + 1);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 1 + 1 + 1 + 1,
                                                             ptr + 8 + 1 + 1 + 1 + 1, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glColorMask: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glColorMask\n", err);
#endif
                DEBUG("gles1(%p): glColorMask(%d %d %d %d )\n", stream, var_red, var_green,
                      var_blue, var_alpha);
                this->glColorMask(var_red, var_green, var_blue, var_alpha);
                SET_LASTCALL("glColorMask");
                break;
            }
            case OP_glColorPointer: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_pointer __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                InputBuffer inptr_pointer(ptr + 8 + 4 + 4 + 4 + 4, size_pointer);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + size_pointer,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + size_pointer,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glColorPointer: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glColorPointer\n", err);
#endif
                DEBUG("gles1(%p): glColorPointer(%d 0x%08x %d %p(%u) )\n", stream, var_size,
                      var_type, var_stride, (const GLvoid *) (inptr_pointer.get()), size_pointer);
                this->glColorPointer(var_size, var_type, var_stride,
                                     (const GLvoid *) (inptr_pointer.get()));
                SET_LASTCALL("glColorPointer");
                break;
            }
            case OP_glCompressedTexImage2D: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLint var_level = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLenum var_internalformat = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                GLsizei var_width = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLsizei var_height = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLint var_border = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                GLsizei var_imageSize = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4);
                uint32_t size_data __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                InputBuffer inptr_data(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4, size_data);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
                                                             size_data,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
                                                             4 + size_data, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glCompressedTexImage2D: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glCompressedTexImage2D\n",
                            err);
#endif
                DEBUG("gles1(%p): glCompressedTexImage2D(0x%08x %d 0x%08x %d %d %d %d %p(%u) )\n",
                      stream, var_target, var_level, var_internalformat, var_width, var_height,
                      var_border, var_imageSize, (const GLvoid *) (inptr_data.get()), size_data);
                this->glCompressedTexImage2D(var_target, var_level, var_internalformat, var_width,
                                             var_height, var_border, var_imageSize,
                                             size_data == 0 ? nullptr
                                                            : (const GLvoid *) (inptr_data.get()));
                SET_LASTCALL("glCompressedTexImage2D");
                break;
            }
            case OP_glCompressedTexSubImage2D: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLint var_level = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLint var_xoffset = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4);
                GLint var_yoffset = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLsizei var_width = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLsizei var_height = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                GLenum var_format = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4);
                GLsizei var_imageSize = Unpack<GLsizei, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                uint32_t size_data __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                InputBuffer inptr_data(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4, size_data);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
                                                             size_data,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
                                                             4 + 4 + size_data, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glCompressedTexSubImage2D: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glCompressedTexSubImage2D\n", err);
#endif
                DEBUG("gles1(%p): glCompressedTexSubImage2D(0x%08x %d %d %d %d %d 0x%08x %d %p(%u) )\n",
                      stream, var_target, var_level, var_xoffset, var_yoffset, var_width,
                      var_height, var_format, var_imageSize, (const GLvoid *) (inptr_data.get()),
                      size_data);
                this->glCompressedTexSubImage2D(var_target, var_level, var_xoffset, var_yoffset,
                                                var_width, var_height, var_format, var_imageSize,
                                                size_data == 0 ? nullptr
                                                               : (const GLvoid *) (inptr_data.get()));
                SET_LASTCALL("glCompressedTexSubImage2D");
                break;
            }
            case OP_glCopyTexImage2D: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLint var_level = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLenum var_internalformat = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                GLint var_x = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLint var_y = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLsizei var_width = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                GLsizei var_height = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4);
                GLint var_border = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
                                                             4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glCopyTexImage2D: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glCopyTexImage2D\n", err);
#endif
                DEBUG("gles1(%p): glCopyTexImage2D(0x%08x %d 0x%08x %d %d %d %d %d )\n", stream,
                      var_target, var_level, var_internalformat, var_x, var_y, var_width,
                      var_height, var_border);
                this->glCopyTexImage2D(var_target, var_level, var_internalformat, var_x, var_y,
                                       var_width, var_height, var_border);
                SET_LASTCALL("glCopyTexImage2D");
                break;
            }
            case OP_glCopyTexSubImage2D: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLint var_level = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLint var_xoffset = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4);
                GLint var_yoffset = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLint var_x = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLint var_y = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                GLsizei var_width = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4);
                GLsizei var_height = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
                                                             4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glCopyTexSubImage2D: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glCopyTexSubImage2D\n",
                            err);
#endif
                DEBUG("gles1(%p): glCopyTexSubImage2D(0x%08x %d %d %d %d %d %d %d )\n", stream,
                      var_target, var_level, var_xoffset, var_yoffset, var_x, var_y, var_width,
                      var_height);
                this->glCopyTexSubImage2D(var_target, var_level, var_xoffset, var_yoffset, var_x,
                                          var_y, var_width, var_height);
                SET_LASTCALL("glCopyTexSubImage2D");
                break;
            }
            case OP_glCullFace: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glCullFace: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glCullFace\n", err);
#endif
                DEBUG("gles1(%p): glCullFace(0x%08x )\n", stream, var_mode);
                this->glCullFace(var_mode);
                SET_LASTCALL("glCullFace");
                break;
            }
            case OP_glDeleteBuffers: {
                GLsizei var_n = Unpack<GLsizei, uint32_t>(ptr + 8);
                uint32_t size_buffers __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_buffers(ptr + 8 + 4 + 4, size_buffers);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_buffers,
                                                             ptr + 8 + 4 + 4 + size_buffers,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDeleteBuffers: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDeleteBuffers\n", err);
#endif
                DEBUG("gles1(%p): glDeleteBuffers(%d %p(%u) )\n", stream, var_n,
                      (const GLuint *) (inptr_buffers.get()), size_buffers);
                this->glDeleteBuffers_dec(this, var_n, (const GLuint *) (inptr_buffers.get()));
                SET_LASTCALL("glDeleteBuffers");
                break;
            }
            case OP_glDeleteTextures: {
                GLsizei var_n = Unpack<GLsizei, uint32_t>(ptr + 8);
                uint32_t size_textures __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_textures(ptr + 8 + 4 + 4, size_textures);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_textures,
                                                             ptr + 8 + 4 + 4 + size_textures,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDeleteTextures: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDeleteTextures\n", err);
#endif
                DEBUG("gles1(%p): glDeleteTextures(%d %p(%u) )\n", stream, var_n,
                      (const GLuint *) (inptr_textures.get()), size_textures);
                this->glDeleteTextures_dec(this, var_n, (const GLuint *) (inptr_textures.get()));
                SET_LASTCALL("glDeleteTextures");
                break;
            }
            case OP_glDepthFunc: {
                GLenum var_func = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDepthFunc: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDepthFunc\n", err);
#endif
                DEBUG("gles1(%p): glDepthFunc(0x%08x )\n", stream, var_func);
                this->glDepthFunc(var_func);
                SET_LASTCALL("glDepthFunc");
                break;
            }
            case OP_glDepthMask: {
                GLboolean var_flag = Unpack<GLboolean, uint8_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 1, ptr + 8 + 1,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDepthMask: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDepthMask\n", err);
#endif
                DEBUG("gles1(%p): glDepthMask(%d )\n", stream, var_flag);
                this->glDepthMask(var_flag);
                SET_LASTCALL("glDepthMask");
                break;
            }
            case OP_glDepthRangex: {
                GLclampx var_zNear = Unpack<GLclampx, uint32_t>(ptr + 8);
                GLclampx var_zFar = Unpack<GLclampx, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDepthRangex: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDepthRangex\n", err);
#endif
                DEBUG("gles1(%p): glDepthRangex(0x%08x 0x%08x )\n", stream, var_zNear, var_zFar);
                this->glDepthRangex(var_zNear, var_zFar);
                SET_LASTCALL("glDepthRangex");
                break;
            }
            case OP_glDisable: {
                GLenum var_cap = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDisable: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDisable\n", err);
#endif
                DEBUG("gles1(%p): glDisable(0x%08x )\n", stream, var_cap);
                this->glDisable(var_cap);
                SET_LASTCALL("glDisable");
                break;
            }
            case OP_glDisableClientState: {
                GLenum var_array = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDisableClientState: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDisableClientState\n",
                            err);
#endif
                DEBUG("gles1(%p): glDisableClientState(0x%08x )\n", stream, var_array);
                this->glDisableClientState(var_array);
                SET_LASTCALL("glDisableClientState");
                break;
            }
            case OP_glDrawArrays: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                GLint var_first = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLsizei var_count = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDrawArrays: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDrawArrays\n", err);
#endif
                DEBUG("gles1(%p): glDrawArrays(0x%08x %d %d )\n", stream, var_mode, var_first,
                      var_count);
                this->glDrawArrays(var_mode, var_first, var_count);
                SET_LASTCALL("glDrawArrays");
                break;
            }
            case OP_glDrawElements: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                GLsizei var_count = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_indices __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                InputBuffer inptr_indices(ptr + 8 + 4 + 4 + 4 + 4, size_indices);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + size_indices,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + size_indices,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDrawElements: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDrawElements\n", err);
#endif
                DEBUG("gles1(%p): glDrawElements(0x%08x %d 0x%08x %p(%u) )\n", stream, var_mode,
                      var_count, var_type, (const GLvoid *) (inptr_indices.get()), size_indices);
                this->glDrawElements(var_mode, var_count, var_type,
                                     (const GLvoid *) (inptr_indices.get()));
                SET_LASTCALL("glDrawElements");
                break;
            }
            case OP_glEnable: {
                GLenum var_cap = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glEnable: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glEnable\n", err);
#endif
                DEBUG("gles1(%p): glEnable(0x%08x )\n", stream, var_cap);
                this->glEnable(var_cap);
                SET_LASTCALL("glEnable");
                break;
            }
            case OP_glEnableClientState: {
                GLenum var_array = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glEnableClientState: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glEnableClientState\n",
                            err);
#endif
                DEBUG("gles1(%p): glEnableClientState(0x%08x )\n", stream, var_array);
                this->glEnableClientState(var_array);
                SET_LASTCALL("glEnableClientState");
                break;
            }
            case OP_glFinish: {
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8, ptr + 8,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFinish: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFinish\n", err);
#endif
                DEBUG("gles1(%p): glFinish()\n", stream);
                this->glFinish();
                SET_LASTCALL("glFinish");
                break;
            }
            case OP_glFlush: {
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8, ptr + 8,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFlush: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFlush\n", err);
#endif
                DEBUG("gles1(%p): glFlush()\n", stream);
                this->glFlush();
                SET_LASTCALL("glFlush");
                break;
            }
            case OP_glFogx: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFogx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFogx\n", err);
#endif
                DEBUG("gles1(%p): glFogx(0x%08x 0x%08x )\n", stream, var_pname, var_param);
                this->glFogx(var_pname, var_param);
                SET_LASTCALL("glFogx");
                break;
            }
            case OP_glFogxv: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFogxv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFogxv\n", err);
#endif
                DEBUG("gles1(%p): glFogxv(0x%08x %p(%u) )\n", stream, var_pname,
                      (const GLfixed *) (inptr_params.get()), size_params);
                this->glFogxv(var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glFogxv");
                break;
            }
            case OP_glFrontFace: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFrontFace: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFrontFace\n", err);
#endif
                DEBUG("gles1(%p): glFrontFace(0x%08x )\n", stream, var_mode);
                this->glFrontFace(var_mode);
                SET_LASTCALL("glFrontFace");
                break;
            }
            case OP_glFrustumx: {
                GLfixed var_left = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_right = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_bottom = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                GLfixed var_top = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfixed var_zNear = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLfixed var_zFar = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFrustumx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFrustumx\n", err);
#endif
                DEBUG("gles1(%p): glFrustumx(0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x )\n", stream,
                      var_left, var_right, var_bottom, var_top, var_zNear, var_zFar);
                this->glFrustumx(var_left, var_right, var_bottom, var_top, var_zNear, var_zFar);
                SET_LASTCALL("glFrustumx");
                break;
            }
            case OP_glGetBooleanv: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetBooleanv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetBooleanv\n", err);
#endif
                DEBUG("gles1(%p): glGetBooleanv(0x%08x %p(%u) )\n", stream, var_pname,
                      (GLboolean *) (outptr_params.get()), size_params);
                this->glGetBooleanv(var_pname, (GLboolean *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetBooleanv");
                break;
            }
            case OP_glGetBufferParameteriv: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetBufferParameteriv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetBufferParameteriv\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetBufferParameteriv(0x%08x 0x%08x %p(%u) )\n", stream,
                      var_target, var_pname, (GLint *) (outptr_params.get()), size_params);
                this->glGetBufferParameteriv(var_target, var_pname,
                                             (GLint *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetBufferParameteriv");
                break;
            }
            case OP_glClipPlanex: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_eqn __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8 + 4);
                InputBuffer inptr_eqn(ptr + 8 + 4 + 4, size_eqn);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_eqn,
                                                             ptr + 8 + 4 + 4 + size_eqn,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClipPlanex: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClipPlanex\n", err);
#endif
                DEBUG("gles1(%p): glClipPlanex(0x%08x %p(%u) )\n", stream, var_pname,
                      (const GLfixed *) (inptr_eqn.get()), size_eqn);
                this->glClipPlanex(var_pname, (const GLfixed *) (inptr_eqn.get()));
                SET_LASTCALL("glClipPlanex");
                break;
            }
            case OP_glGenBuffers: {
                GLsizei var_n = Unpack<GLsizei, uint32_t>(ptr + 8);
                uint32_t size_buffers __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGenBuffers: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_buffers;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_buffers(&tmpBuf[0], size_buffers);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGenBuffers\n", err);
#endif
                DEBUG("gles1(%p): glGenBuffers(%d %p(%u) )\n", stream, var_n,
                      (GLuint *) (outptr_buffers.get()), size_buffers);
                this->glGenBuffers_dec(this, var_n, (GLuint *) (outptr_buffers.get()));
                outptr_buffers.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGenBuffers");
                break;
            }
            case OP_glGenTextures: {
                GLsizei var_n = Unpack<GLsizei, uint32_t>(ptr + 8);
                uint32_t size_textures __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGenTextures: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_textures;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_textures(&tmpBuf[0], size_textures);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGenTextures\n", err);
#endif
                DEBUG("gles1(%p): glGenTextures(%d %p(%u) )\n", stream, var_n,
                      (GLuint *) (outptr_textures.get()), size_textures);
                this->glGenTextures_dec(this, var_n, (GLuint *) (outptr_textures.get()));
                outptr_textures.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGenTextures");
                break;
            }
            case OP_glGetError: {
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8, ptr + 8,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetError: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(GLenum);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetError\n", err);
#endif
                DEBUG("gles1(%p): glGetError()\n", stream);
                *(GLenum *) (&tmpBuf[0]) = this->glGetError();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetError");
                break;
            }
            case OP_glGetFixedv: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetFixedv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetFixedv\n", err);
#endif
                DEBUG("gles1(%p): glGetFixedv(0x%08x %p(%u) )\n", stream, var_pname,
                      (GLfixed *) (outptr_params.get()), size_params);
                this->glGetFixedv(var_pname, (GLfixed *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetFixedv");
                break;
            }
            case OP_glGetIntegerv: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetIntegerv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetIntegerv\n", err);
#endif
                DEBUG("gles1(%p): glGetIntegerv(0x%08x %p(%u) )\n", stream, var_pname,
                      (GLint *) (outptr_params.get()), size_params);
                this->glGetIntegerv(var_pname, (GLint *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetIntegerv");
                break;
            }
            case OP_glGetLightxv: {
                GLenum var_light = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetLightxv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetLightxv\n", err);
#endif
                DEBUG("gles1(%p): glGetLightxv(0x%08x 0x%08x %p(%u) )\n", stream, var_light,
                      var_pname, (GLfixed *) (outptr_params.get()), size_params);
                this->glGetLightxv(var_light, var_pname, (GLfixed *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetLightxv");
                break;
            }
            case OP_glGetMaterialxv: {
                GLenum var_face = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetMaterialxv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetMaterialxv\n", err);
#endif
                DEBUG("gles1(%p): glGetMaterialxv(0x%08x 0x%08x %p(%u) )\n", stream, var_face,
                      var_pname, (GLfixed *) (outptr_params.get()), size_params);
                this->glGetMaterialxv(var_face, var_pname, (GLfixed *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetMaterialxv");
                break;
            }
            case OP_glGetPointerv: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetPointerv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetPointerv\n", err);
#endif
                DEBUG("gles1(%p): glGetPointerv(0x%08x %p(%u) )\n", stream, var_pname,
                      (GLvoid **) (inptr_params.get()), size_params);
                this->glGetPointerv(var_pname, (GLvoid **) (inptr_params.get()));
                SET_LASTCALL("glGetPointerv");
                break;
            }
            case OP_glGetString: {
                GLenum var_name = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetString: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetString\n", err);
#endif
                DEBUG("gles1(%p): glGetString(0x%08x )\n", stream, var_name);
                this->glGetString(var_name);
                SET_LASTCALL("glGetString");
                break;
            }
            case OP_glGetTexEnviv: {
                GLenum var_env = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetTexEnviv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetTexEnviv\n", err);
#endif
                DEBUG("gles1(%p): glGetTexEnviv(0x%08x 0x%08x %p(%u) )\n", stream, var_env,
                      var_pname, (GLint *) (outptr_params.get()), size_params);
                this->glGetTexEnviv(var_env, var_pname, (GLint *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetTexEnviv");
                break;
            }
            case OP_glGetTexEnvxv: {
                GLenum var_env = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetTexEnvxv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetTexEnvxv\n", err);
#endif
                DEBUG("gles1(%p): glGetTexEnvxv(0x%08x 0x%08x %p(%u) )\n", stream, var_env,
                      var_pname, (GLfixed *) (outptr_params.get()), size_params);
                this->glGetTexEnvxv(var_env, var_pname, (GLfixed *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetTexEnvxv");
                break;
            }
            case OP_glGetTexParameteriv: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetTexParameteriv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetTexParameteriv\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetTexParameteriv(0x%08x 0x%08x %p(%u) )\n", stream, var_target,
                      var_pname, (GLint *) (outptr_params.get()), size_params);
                this->glGetTexParameteriv(var_target, var_pname, (GLint *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetTexParameteriv");
                break;
            }
            case OP_glGetTexParameterxv: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetTexParameterxv: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetTexParameterxv\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetTexParameterxv(0x%08x 0x%08x %p(%u) )\n", stream, var_target,
                      var_pname, (GLfixed *) (outptr_params.get()), size_params);
                this->glGetTexParameterxv(var_target, var_pname, (GLfixed *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetTexParameterxv");
                break;
            }
            case OP_glHint: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glHint: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glHint\n", err);
#endif
                DEBUG("gles1(%p): glHint(0x%08x 0x%08x )\n", stream, var_target, var_mode);
                this->glHint(var_target, var_mode);
                SET_LASTCALL("glHint");
                break;
            }
            case OP_glIsBuffer: {
                GLuint var_buffer = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glIsBuffer: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(GLboolean);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glIsBuffer\n", err);
#endif
                DEBUG("gles1(%p): glIsBuffer(%u )\n", stream, var_buffer);
                *(GLboolean *) (&tmpBuf[0]) = this->glIsBuffer(var_buffer);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glIsBuffer");
                break;
            }
            case OP_glIsEnabled: {
                GLenum var_cap = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glIsEnabled: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(GLboolean);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glIsEnabled\n", err);
#endif
                DEBUG("gles1(%p): glIsEnabled(0x%08x )\n", stream, var_cap);
                *(GLboolean *) (&tmpBuf[0]) = this->glIsEnabled(var_cap);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glIsEnabled");
                break;
            }
            case OP_glIsTexture: {
                GLuint var_texture = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glIsTexture: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(GLboolean);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glIsTexture\n", err);
#endif
                DEBUG("gles1(%p): glIsTexture(%u )\n", stream, var_texture);
                *(GLboolean *) (&tmpBuf[0]) = this->glIsTexture(var_texture);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glIsTexture");
                break;
            }
            case OP_glLightModelx: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLightModelx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLightModelx\n", err);
#endif
                DEBUG("gles1(%p): glLightModelx(0x%08x 0x%08x )\n", stream, var_pname, var_param);
                this->glLightModelx(var_pname, var_param);
                SET_LASTCALL("glLightModelx");
                break;
            }
            case OP_glLightModelxv: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLightModelxv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLightModelxv\n", err);
#endif
                DEBUG("gles1(%p): glLightModelxv(0x%08x %p(%u) )\n", stream, var_pname,
                      (const GLfixed *) (inptr_params.get()), size_params);
                this->glLightModelxv(var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glLightModelxv");
                break;
            }
            case OP_glLightx: {
                GLenum var_light = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLightx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLightx\n", err);
#endif
                DEBUG("gles1(%p): glLightx(0x%08x 0x%08x 0x%08x )\n", stream, var_light, var_pname,
                      var_param);
                this->glLightx(var_light, var_pname, var_param);
                SET_LASTCALL("glLightx");
                break;
            }
            case OP_glLightxv: {
                GLenum var_light = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLightxv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLightxv\n", err);
#endif
                DEBUG("gles1(%p): glLightxv(0x%08x 0x%08x %p(%u) )\n", stream, var_light, var_pname,
                      (const GLfixed *) (inptr_params.get()), size_params);
                this->glLightxv(var_light, var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glLightxv");
                break;
            }
            case OP_glLineWidthx: {
                GLfixed var_width = Unpack<GLfixed, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLineWidthx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLineWidthx\n", err);
#endif
                DEBUG("gles1(%p): glLineWidthx(0x%08x )\n", stream, var_width);
                this->glLineWidthx(var_width);
                SET_LASTCALL("glLineWidthx");
                break;
            }
            case OP_glLoadIdentity: {
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8, ptr + 8,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLoadIdentity: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLoadIdentity\n", err);
#endif
                DEBUG("gles1(%p): glLoadIdentity()\n", stream);
                this->glLoadIdentity();
                SET_LASTCALL("glLoadIdentity");
                break;
            }
            case OP_glLoadMatrixx: {
                uint32_t size_m __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                InputBuffer inptr_m(ptr + 8 + 4, size_m);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + size_m,
                                                             ptr + 8 + 4 + size_m, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLoadMatrixx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLoadMatrixx\n", err);
#endif
                DEBUG("gles1(%p): glLoadMatrixx(%p(%u) )\n", stream,
                      (const GLfixed *) (inptr_m.get()), size_m);
                this->glLoadMatrixx((const GLfixed *) (inptr_m.get()));
                SET_LASTCALL("glLoadMatrixx");
                break;
            }
            case OP_glLogicOp: {
                GLenum var_opcode = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLogicOp: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLogicOp\n", err);
#endif
                DEBUG("gles1(%p): glLogicOp(0x%08x )\n", stream, var_opcode);
                this->glLogicOp(var_opcode);
                SET_LASTCALL("glLogicOp");
                break;
            }
            case OP_glMaterialx: {
                GLenum var_face = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMaterialx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMaterialx\n", err);
#endif
                DEBUG("gles1(%p): glMaterialx(0x%08x 0x%08x 0x%08x )\n", stream, var_face,
                      var_pname, var_param);
                this->glMaterialx(var_face, var_pname, var_param);
                SET_LASTCALL("glMaterialx");
                break;
            }
            case OP_glMaterialxv: {
                GLenum var_face = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMaterialxv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMaterialxv\n", err);
#endif
                DEBUG("gles1(%p): glMaterialxv(0x%08x 0x%08x %p(%u) )\n", stream, var_face,
                      var_pname, (const GLfixed *) (inptr_params.get()), size_params);
                this->glMaterialxv(var_face, var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glMaterialxv");
                break;
            }
            case OP_glMatrixMode: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMatrixMode: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMatrixMode\n", err);
#endif
                DEBUG("gles1(%p): glMatrixMode(0x%08x )\n", stream, var_mode);
                this->glMatrixMode(var_mode);
                SET_LASTCALL("glMatrixMode");
                break;
            }
            case OP_glMultMatrixx: {
                uint32_t size_m __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                InputBuffer inptr_m(ptr + 8 + 4, size_m);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + size_m,
                                                             ptr + 8 + 4 + size_m, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMultMatrixx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMultMatrixx\n", err);
#endif
                DEBUG("gles1(%p): glMultMatrixx(%p(%u) )\n", stream,
                      (const GLfixed *) (inptr_m.get()), size_m);
                this->glMultMatrixx((const GLfixed *) (inptr_m.get()));
                SET_LASTCALL("glMultMatrixx");
                break;
            }
            case OP_glMultiTexCoord4x: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLfixed var_s = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_t = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                GLfixed var_r = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfixed var_q = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMultiTexCoord4x: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMultiTexCoord4x\n", err);
#endif
                DEBUG("gles1(%p): glMultiTexCoord4x(0x%08x 0x%08x 0x%08x 0x%08x 0x%08x )\n", stream,
                      var_target, var_s, var_t, var_r, var_q);
                this->glMultiTexCoord4x(var_target, var_s, var_t, var_r, var_q);
                SET_LASTCALL("glMultiTexCoord4x");
                break;
            }
            case OP_glNormal3x: {
                GLfixed var_nx = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_ny = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_nz = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glNormal3x: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glNormal3x\n", err);
#endif
                DEBUG("gles1(%p): glNormal3x(0x%08x 0x%08x 0x%08x )\n", stream, var_nx, var_ny,
                      var_nz);
                this->glNormal3x(var_nx, var_ny, var_nz);
                SET_LASTCALL("glNormal3x");
                break;
            }
            case OP_glNormalPointer: {
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                uint32_t size_pointer __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_pointer(ptr + 8 + 4 + 4 + 4, size_pointer);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_pointer,
                                                             ptr + 8 + 4 + 4 + 4 + size_pointer,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glNormalPointer: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glNormalPointer\n", err);
#endif
                DEBUG("gles1(%p): glNormalPointer(0x%08x %d %p(%u) )\n", stream, var_type,
                      var_stride, (const GLvoid *) (inptr_pointer.get()), size_pointer);
                this->glNormalPointer(var_type, var_stride, (const GLvoid *) (inptr_pointer.get()));
                SET_LASTCALL("glNormalPointer");
                break;
            }
            case OP_glOrthox: {
                GLfixed var_left = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_right = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_bottom = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                GLfixed var_top = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfixed var_zNear = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLfixed var_zFar = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glOrthox: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glOrthox\n", err);
#endif
                DEBUG("gles1(%p): glOrthox(0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x )\n", stream,
                      var_left, var_right, var_bottom, var_top, var_zNear, var_zFar);
                this->glOrthox(var_left, var_right, var_bottom, var_top, var_zNear, var_zFar);
                SET_LASTCALL("glOrthox");
                break;
            }
            case OP_glPixelStorei: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                GLint var_param = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPixelStorei: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPixelStorei\n", err);
#endif
                DEBUG("gles1(%p): glPixelStorei(0x%08x %d )\n", stream, var_pname, var_param);
                this->glPixelStorei(var_pname, var_param);
                SET_LASTCALL("glPixelStorei");
                break;
            }
            case OP_glPointParameterx: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPointParameterx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPointParameterx\n", err);
#endif
                DEBUG("gles1(%p): glPointParameterx(0x%08x 0x%08x )\n", stream, var_pname,
                      var_param);
                this->glPointParameterx(var_pname, var_param);
                SET_LASTCALL("glPointParameterx");
                break;
            }
            case OP_glPointParameterxv: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPointParameterxv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPointParameterxv\n",
                            err);
#endif
                DEBUG("gles1(%p): glPointParameterxv(0x%08x %p(%u) )\n", stream, var_pname,
                      (const GLfixed *) (inptr_params.get()), size_params);
                this->glPointParameterxv(var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glPointParameterxv");
                break;
            }
            case OP_glPointSizex: {
                GLfixed var_size = Unpack<GLfixed, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPointSizex: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPointSizex\n", err);
#endif
                DEBUG("gles1(%p): glPointSizex(0x%08x )\n", stream, var_size);
                this->glPointSizex(var_size);
                SET_LASTCALL("glPointSizex");
                break;
            }
            case OP_glPolygonOffsetx: {
                GLfixed var_factor = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_units = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPolygonOffsetx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPolygonOffsetx\n", err);
#endif
                DEBUG("gles1(%p): glPolygonOffsetx(0x%08x 0x%08x )\n", stream, var_factor,
                      var_units);
                this->glPolygonOffsetx(var_factor, var_units);
                SET_LASTCALL("glPolygonOffsetx");
                break;
            }
            case OP_glPopMatrix: {
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8, ptr + 8,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPopMatrix: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPopMatrix\n", err);
#endif
                DEBUG("gles1(%p): glPopMatrix()\n", stream);
                this->glPopMatrix();
                SET_LASTCALL("glPopMatrix");
                break;
            }
            case OP_glPushMatrix: {
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8, ptr + 8,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPushMatrix: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPushMatrix\n", err);
#endif
                DEBUG("gles1(%p): glPushMatrix()\n", stream);
                this->glPushMatrix();
                SET_LASTCALL("glPushMatrix");
                break;
            }
            case OP_glReadPixels: {
                GLint var_x = Unpack<GLint, uint32_t>(ptr + 8);
                GLint var_y = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLsizei var_width = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                GLsizei var_height = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLenum var_format = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                uint32_t size_pixels __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glReadPixels: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_pixels;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_pixels(&tmpBuf[0], size_pixels);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glReadPixels\n", err);
#endif
                DEBUG("gles1(%p): glReadPixels(%d %d %d %d 0x%08x 0x%08x %p(%u) )\n", stream, var_x,
                      var_y, var_width, var_height, var_format, var_type,
                      (GLvoid *) (outptr_pixels.get()), size_pixels);
                this->glReadPixels(var_x, var_y, var_width, var_height, var_format, var_type,
                                   (GLvoid *) (outptr_pixels.get()));
                outptr_pixels.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glReadPixels");
                break;
            }
            case OP_glRotatex: {
                GLfixed var_angle = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_x = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_y = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                GLfixed var_z = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glRotatex: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glRotatex\n", err);
#endif
                DEBUG("gles1(%p): glRotatex(0x%08x 0x%08x 0x%08x 0x%08x )\n", stream, var_angle,
                      var_x, var_y, var_z);
                this->glRotatex(var_angle, var_x, var_y, var_z);
                SET_LASTCALL("glRotatex");
                break;
            }
            case OP_glSampleCoverage: {
                GLclampf var_value = Unpack<GLclampf, uint32_t>(ptr + 8);
                GLboolean var_invert = Unpack<GLboolean, uint8_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 1,
                                                             ptr + 8 + 4 + 1, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glSampleCoverage: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glSampleCoverage\n", err);
#endif
                DEBUG("gles1(%p): glSampleCoverage(%f %d )\n", stream, var_value, var_invert);
                this->glSampleCoverage(var_value, var_invert);
                SET_LASTCALL("glSampleCoverage");
                break;
            }
            case OP_glSampleCoveragex: {
                GLclampx var_value = Unpack<GLclampx, uint32_t>(ptr + 8);
                GLboolean var_invert = Unpack<GLboolean, uint8_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 1,
                                                             ptr + 8 + 4 + 1, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glSampleCoveragex: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glSampleCoveragex\n", err);
#endif
                DEBUG("gles1(%p): glSampleCoveragex(0x%08x %d )\n", stream, var_value, var_invert);
                this->glSampleCoveragex(var_value, var_invert);
                SET_LASTCALL("glSampleCoveragex");
                break;
            }
            case OP_glScalex: {
                GLfixed var_x = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_y = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_z = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glScalex: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glScalex\n", err);
#endif
                DEBUG("gles1(%p): glScalex(0x%08x 0x%08x 0x%08x )\n", stream, var_x, var_y, var_z);
                this->glScalex(var_x, var_y, var_z);
                SET_LASTCALL("glScalex");
                break;
            }
            case OP_glScissor: {
                GLint var_x = Unpack<GLint, uint32_t>(ptr + 8);
                GLint var_y = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLsizei var_width = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                GLsizei var_height = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glScissor: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glScissor\n", err);
#endif
                DEBUG("gles1(%p): glScissor(%d %d %d %d )\n", stream, var_x, var_y, var_width,
                      var_height);
                this->glScissor(var_x, var_y, var_width, var_height);
                SET_LASTCALL("glScissor");
                break;
            }
            case OP_glShadeModel: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glShadeModel: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glShadeModel\n", err);
#endif
                DEBUG("gles1(%p): glShadeModel(0x%08x )\n", stream, var_mode);
                this->glShadeModel(var_mode);
                SET_LASTCALL("glShadeModel");
                break;
            }
            case OP_glStencilFunc: {
                GLenum var_func = Unpack<GLenum, uint32_t>(ptr + 8);
                GLint var_ref = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLuint var_mask = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glStencilFunc: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glStencilFunc\n", err);
#endif
                DEBUG("gles1(%p): glStencilFunc(0x%08x %d %u )\n", stream, var_func, var_ref,
                      var_mask);
                this->glStencilFunc(var_func, var_ref, var_mask);
                SET_LASTCALL("glStencilFunc");
                break;
            }
            case OP_glStencilMask: {
                GLuint var_mask = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glStencilMask: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glStencilMask\n", err);
#endif
                DEBUG("gles1(%p): glStencilMask(%u )\n", stream, var_mask);
                this->glStencilMask(var_mask);
                SET_LASTCALL("glStencilMask");
                break;
            }
            case OP_glStencilOp: {
                GLenum var_fail = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_zfail = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLenum var_zpass = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glStencilOp: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glStencilOp\n", err);
#endif
                DEBUG("gles1(%p): glStencilOp(0x%08x 0x%08x 0x%08x )\n", stream, var_fail,
                      var_zfail, var_zpass);
                this->glStencilOp(var_fail, var_zfail, var_zpass);
                SET_LASTCALL("glStencilOp");
                break;
            }
            case OP_glTexCoordPointer: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_pointer __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                InputBuffer inptr_pointer(ptr + 8 + 4 + 4 + 4 + 4, size_pointer);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + size_pointer,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + size_pointer,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexCoordPointer: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexCoordPointer\n", err);
#endif
                DEBUG("gles1(%p): glTexCoordPointer(%d 0x%08x %d %p(%u) )\n", stream, var_size,
                      var_type, var_stride, (const GLvoid *) (inptr_pointer.get()), size_pointer);
                this->glTexCoordPointer(var_size, var_type, var_stride,
                                        (const GLvoid *) (inptr_pointer.get()));
                SET_LASTCALL("glTexCoordPointer");
                break;
            }
            case OP_glTexEnvi: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLint var_param = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexEnvi: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexEnvi\n", err);
#endif
                DEBUG("gles1(%p): glTexEnvi(0x%08x 0x%08x %d )\n", stream, var_target, var_pname,
                      var_param);
                this->glTexEnvi(var_target, var_pname, var_param);
                SET_LASTCALL("glTexEnvi");
                break;
            }
            case OP_glTexEnvx: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexEnvx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexEnvx\n", err);
#endif
                DEBUG("gles1(%p): glTexEnvx(0x%08x 0x%08x 0x%08x )\n", stream, var_target,
                      var_pname, var_param);
                this->glTexEnvx(var_target, var_pname, var_param);
                SET_LASTCALL("glTexEnvx");
                break;
            }
            case OP_glTexEnviv: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexEnviv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexEnviv\n", err);
#endif
                DEBUG("gles1(%p): glTexEnviv(0x%08x 0x%08x %p(%u) )\n", stream, var_target,
                      var_pname, (const GLint *) (inptr_params.get()), size_params);
                this->glTexEnviv(var_target, var_pname, (const GLint *) (inptr_params.get()));
                SET_LASTCALL("glTexEnviv");
                break;
            }
            case OP_glTexEnvxv: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexEnvxv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexEnvxv\n", err);
#endif
                DEBUG("gles1(%p): glTexEnvxv(0x%08x 0x%08x %p(%u) )\n", stream, var_target,
                      var_pname, (const GLfixed *) (inptr_params.get()), size_params);
                this->glTexEnvxv(var_target, var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glTexEnvxv");
                break;
            }
            case OP_glTexImage2D: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLint var_level = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLint var_internalformat = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4);
                GLsizei var_width = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLsizei var_height = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLint var_border = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                GLenum var_format = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                uint32_t size_pixels __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                InputBuffer inptr_pixels(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4, size_pixels);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
                                                             size_pixels,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
                                                             4 + 4 + size_pixels, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexImage2D: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexImage2D\n", err);
#endif
                DEBUG("gles1(%p): glTexImage2D(0x%08x %d %d %d %d %d 0x%08x 0x%08x %p(%u) )\n",
                      stream, var_target, var_level, var_internalformat, var_width, var_height,
                      var_border, var_format, var_type, (const GLvoid *) (inptr_pixels.get()),
                      size_pixels);
                this->glTexImage2D(var_target, var_level, var_internalformat, var_width, var_height,
                                   var_border, var_format, var_type, size_pixels == 0 ? nullptr
                                                                                      : (const GLvoid *) (inptr_pixels.get()));
                SET_LASTCALL("glTexImage2D");
                break;
            }
            case OP_glTexParameteri: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLint var_param = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexParameteri: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexParameteri\n", err);
#endif
                DEBUG("gles1(%p): glTexParameteri(0x%08x 0x%08x %d )\n", stream, var_target,
                      var_pname, var_param);
                this->glTexParameteri(var_target, var_pname, var_param);
                SET_LASTCALL("glTexParameteri");
                break;
            }
            case OP_glTexParameterx: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexParameterx: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexParameterx\n", err);
#endif
                DEBUG("gles1(%p): glTexParameterx(0x%08x 0x%08x 0x%08x )\n", stream, var_target,
                      var_pname, var_param);
                this->glTexParameterx(var_target, var_pname, var_param);
                SET_LASTCALL("glTexParameterx");
                break;
            }
            case OP_glTexParameteriv: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexParameteriv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexParameteriv\n", err);
#endif
                DEBUG("gles1(%p): glTexParameteriv(0x%08x 0x%08x %p(%u) )\n", stream, var_target,
                      var_pname, (const GLint *) (inptr_params.get()), size_params);
                this->glTexParameteriv(var_target, var_pname, (const GLint *) (inptr_params.get()));
                SET_LASTCALL("glTexParameteriv");
                break;
            }
            case OP_glTexParameterxv: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexParameterxv: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexParameterxv\n", err);
#endif
                DEBUG("gles1(%p): glTexParameterxv(0x%08x 0x%08x %p(%u) )\n", stream, var_target,
                      var_pname, (const GLfixed *) (inptr_params.get()), size_params);
                this->glTexParameterxv(var_target, var_pname,
                                       (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glTexParameterxv");
                break;
            }
            case OP_glTexSubImage2D: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLint var_level = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLint var_xoffset = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4);
                GLint var_yoffset = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLsizei var_width = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLsizei var_height = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                GLenum var_format = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                uint32_t size_pixels __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                InputBuffer inptr_pixels(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4, size_pixels);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
                                                             size_pixels,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
                                                             4 + 4 + size_pixels, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexSubImage2D: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexSubImage2D\n", err);
#endif
                DEBUG("gles1(%p): glTexSubImage2D(0x%08x %d %d %d %d %d 0x%08x 0x%08x %p(%u) )\n",
                      stream, var_target, var_level, var_xoffset, var_yoffset, var_width,
                      var_height, var_format, var_type, (const GLvoid *) (inptr_pixels.get()),
                      size_pixels);
                this->glTexSubImage2D(var_target, var_level, var_xoffset, var_yoffset, var_width,
                                      var_height, var_format, var_type, size_pixels == 0 ? nullptr
                                                                                         : (const GLvoid *) (inptr_pixels.get()));
                SET_LASTCALL("glTexSubImage2D");
                break;
            }
            case OP_glTranslatex: {
                GLfixed var_x = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_y = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_z = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTranslatex: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTranslatex\n", err);
#endif
                DEBUG("gles1(%p): glTranslatex(0x%08x 0x%08x 0x%08x )\n", stream, var_x, var_y,
                      var_z);
                this->glTranslatex(var_x, var_y, var_z);
                SET_LASTCALL("glTranslatex");
                break;
            }
            case OP_glVertexPointer: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_pointer __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                InputBuffer inptr_pointer(ptr + 8 + 4 + 4 + 4 + 4, size_pointer);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + size_pointer,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + size_pointer,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glVertexPointer: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glVertexPointer\n", err);
#endif
                DEBUG("gles1(%p): glVertexPointer(%d 0x%08x %d %p(%u) )\n", stream, var_size,
                      var_type, var_stride, (const GLvoid *) (inptr_pointer.get()), size_pointer);
                this->glVertexPointer(var_size, var_type, var_stride,
                                      (const GLvoid *) (inptr_pointer.get()));
                SET_LASTCALL("glVertexPointer");
                break;
            }
            case OP_glViewport: {
                GLint var_x = Unpack<GLint, uint32_t>(ptr + 8);
                GLint var_y = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLsizei var_width = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                GLsizei var_height = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glViewport: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glViewport\n", err);
#endif
                DEBUG("gles1(%p): glViewport(%d %d %d %d )\n", stream, var_x, var_y, var_width,
                      var_height);
                this->glViewport(var_x, var_y, var_width, var_height);
                SET_LASTCALL("glViewport");
                break;
            }
            case OP_glPointSizePointerOES: {
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                uint32_t size_pointer __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_pointer(ptr + 8 + 4 + 4 + 4, size_pointer);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_pointer,
                                                             ptr + 8 + 4 + 4 + 4 + size_pointer,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPointSizePointerOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPointSizePointerOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glPointSizePointerOES(0x%08x %d %p(%u) )\n", stream, var_type,
                      var_stride, (const GLvoid *) (inptr_pointer.get()), size_pointer);
                this->glPointSizePointerOES(var_type, var_stride,
                                            (const GLvoid *) (inptr_pointer.get()));
                SET_LASTCALL("glPointSizePointerOES");
                break;
            }
            case OP_glVertexPointerOffset: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                GLuint var_offset = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glVertexPointerOffset: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glVertexPointerOffset\n",
                            err);
#endif
                DEBUG("gles1(%p): glVertexPointerOffset(%d 0x%08x %d %u )\n", stream, var_size,
                      var_type, var_stride, var_offset);
                this->glVertexPointerOffset(this, var_size, var_type, var_stride, var_offset);
                SET_LASTCALL("glVertexPointerOffset");
                break;
            }
            case OP_glColorPointerOffset: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                GLuint var_offset = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glColorPointerOffset: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glColorPointerOffset\n",
                            err);
#endif
                DEBUG("gles1(%p): glColorPointerOffset(%d 0x%08x %d %u )\n", stream, var_size,
                      var_type, var_stride, var_offset);
                this->glColorPointerOffset(this, var_size, var_type, var_stride, var_offset);
                SET_LASTCALL("glColorPointerOffset");
                break;
            }
            case OP_glNormalPointerOffset: {
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                GLuint var_offset = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glNormalPointerOffset: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glNormalPointerOffset\n",
                            err);
#endif
                DEBUG("gles1(%p): glNormalPointerOffset(0x%08x %d %u )\n", stream, var_type,
                      var_stride, var_offset);
                this->glNormalPointerOffset(this, var_type, var_stride, var_offset);
                SET_LASTCALL("glNormalPointerOffset");
                break;
            }
            case OP_glPointSizePointerOffset: {
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                GLuint var_offset = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPointSizePointerOffset: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glPointSizePointerOffset\n", err);
#endif
                DEBUG("gles1(%p): glPointSizePointerOffset(0x%08x %d %u )\n", stream, var_type,
                      var_stride, var_offset);
                this->glPointSizePointerOffset(this, var_type, var_stride, var_offset);
                SET_LASTCALL("glPointSizePointerOffset");
                break;
            }
            case OP_glTexCoordPointerOffset: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                GLuint var_offset = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexCoordPointerOffset: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexCoordPointerOffset\n",
                            err);
#endif
                DEBUG("gles1(%p): glTexCoordPointerOffset(%d 0x%08x %d %u )\n", stream, var_size,
                      var_type, var_stride, var_offset);
                this->glTexCoordPointerOffset(this, var_size, var_type, var_stride, var_offset);
                SET_LASTCALL("glTexCoordPointerOffset");
                break;
            }
            case OP_glWeightPointerOffset: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                GLuint var_offset = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glWeightPointerOffset: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glWeightPointerOffset\n",
                            err);
#endif
                DEBUG("gles1(%p): glWeightPointerOffset(%d 0x%08x %d %u )\n", stream, var_size,
                      var_type, var_stride, var_offset);
                this->glWeightPointerOffset(this, var_size, var_type, var_stride, var_offset);
                SET_LASTCALL("glWeightPointerOffset");
                break;
            }
            case OP_glMatrixIndexPointerOffset: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                GLuint var_offset = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMatrixIndexPointerOffset: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glMatrixIndexPointerOffset\n",
                            err);
#endif
                DEBUG("gles1(%p): glMatrixIndexPointerOffset(%d 0x%08x %d %u )\n", stream, var_size,
                      var_type, var_stride, var_offset);
                this->glMatrixIndexPointerOffset(this, var_size, var_type, var_stride, var_offset);
                SET_LASTCALL("glMatrixIndexPointerOffset");
                break;
            }
            case OP_glVertexPointerData: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_data __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                InputBuffer inptr_data(ptr + 8 + 4 + 4 + 4 + 4, size_data);
                GLuint var_datalen = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + size_data);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + size_data + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + size_data +
                                                             4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glVertexPointerData: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glVertexPointerData\n",
                            err);
#endif
                DEBUG("gles1(%p): glVertexPointerData(%d 0x%08x %d %p(%u) %u )\n", stream, var_size,
                      var_type, var_stride, (void *) (inptr_data.get()), size_data, var_datalen);
                this->glVertexPointerData(this, var_size, var_type, var_stride,
                                          (void *) (inptr_data.get()), var_datalen);
                SET_LASTCALL("glVertexPointerData");
                break;
            }
            case OP_glColorPointerData: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_data __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                InputBuffer inptr_data(ptr + 8 + 4 + 4 + 4 + 4, size_data);
                GLuint var_datalen = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + size_data);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + size_data + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + size_data +
                                                             4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glColorPointerData: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glColorPointerData\n",
                            err);
#endif
                DEBUG("gles1(%p): glColorPointerData(%d 0x%08x %d %p(%u) %u )\n", stream, var_size,
                      var_type, var_stride, (void *) (inptr_data.get()), size_data, var_datalen);
                this->glColorPointerData(this, var_size, var_type, var_stride,
                                         (void *) (inptr_data.get()), var_datalen);
                SET_LASTCALL("glColorPointerData");
                break;
            }
            case OP_glNormalPointerData: {
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                uint32_t size_data __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_data(ptr + 8 + 4 + 4 + 4, size_data);
                GLuint var_datalen = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4 + size_data);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_data + 4,
                                                             ptr + 8 + 4 + 4 + 4 + size_data + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glNormalPointerData: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glNormalPointerData\n",
                            err);
#endif
                DEBUG("gles1(%p): glNormalPointerData(0x%08x %d %p(%u) %u )\n", stream, var_type,
                      var_stride, (void *) (inptr_data.get()), size_data, var_datalen);
                this->glNormalPointerData(this, var_type, var_stride, (void *) (inptr_data.get()),
                                          var_datalen);
                SET_LASTCALL("glNormalPointerData");
                break;
            }
            case OP_glTexCoordPointerData: {
                GLint var_unit = Unpack<GLint, uint32_t>(ptr + 8);
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4);
                uint32_t size_data __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + 4);
                InputBuffer inptr_data(ptr + 8 + 4 + 4 + 4 + 4 + 4, size_data);
                GLuint var_datalen = Unpack<GLuint, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + 4 + 4 + size_data);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + size_data + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 +
                                                             size_data + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexCoordPointerData: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexCoordPointerData\n",
                            err);
#endif
                DEBUG("gles1(%p): glTexCoordPointerData(%d %d 0x%08x %d %p(%u) %u )\n", stream,
                      var_unit, var_size, var_type, var_stride, (void *) (inptr_data.get()),
                      size_data, var_datalen);
                this->glTexCoordPointerData(this, var_unit, var_size, var_type, var_stride,
                                            (void *) (inptr_data.get()), var_datalen);
                SET_LASTCALL("glTexCoordPointerData");
                break;
            }
            case OP_glPointSizePointerData: {
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                uint32_t size_data __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_data(ptr + 8 + 4 + 4 + 4, size_data);
                GLuint var_datalen = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4 + size_data);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_data + 4,
                                                             ptr + 8 + 4 + 4 + 4 + size_data + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPointSizePointerData: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPointSizePointerData\n",
                            err);
#endif
                DEBUG("gles1(%p): glPointSizePointerData(0x%08x %d %p(%u) %u )\n", stream, var_type,
                      var_stride, (void *) (inptr_data.get()), size_data, var_datalen);
                this->glPointSizePointerData(this, var_type, var_stride,
                                             (void *) (inptr_data.get()), var_datalen);
                SET_LASTCALL("glPointSizePointerData");
                break;
            }
            case OP_glWeightPointerData: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_data __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                InputBuffer inptr_data(ptr + 8 + 4 + 4 + 4 + 4, size_data);
                GLuint var_datalen = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + size_data);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + size_data + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + size_data +
                                                             4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glWeightPointerData: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glWeightPointerData\n",
                            err);
#endif
                DEBUG("gles1(%p): glWeightPointerData(%d 0x%08x %d %p(%u) %u )\n", stream, var_size,
                      var_type, var_stride, (void *) (inptr_data.get()), size_data, var_datalen);
                this->glWeightPointerData(this, var_size, var_type, var_stride,
                                          (void *) (inptr_data.get()), var_datalen);
                SET_LASTCALL("glWeightPointerData");
                break;
            }
            case OP_glMatrixIndexPointerData: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_data __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                InputBuffer inptr_data(ptr + 8 + 4 + 4 + 4 + 4, size_data);
                GLuint var_datalen = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + size_data);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + size_data + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + size_data +
                                                             4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMatrixIndexPointerData: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glMatrixIndexPointerData\n", err);
#endif
                DEBUG("gles1(%p): glMatrixIndexPointerData(%d 0x%08x %d %p(%u) %u )\n", stream,
                      var_size, var_type, var_stride, (void *) (inptr_data.get()), size_data,
                      var_datalen);
                this->glMatrixIndexPointerData(this, var_size, var_type, var_stride,
                                               (void *) (inptr_data.get()), var_datalen);
                SET_LASTCALL("glMatrixIndexPointerData");
                break;
            }
            case OP_glDrawElementsOffset: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                GLsizei var_count = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                GLuint var_offset = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDrawElementsOffset: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDrawElementsOffset\n",
                            err);
#endif
                DEBUG("gles1(%p): glDrawElementsOffset(0x%08x %d 0x%08x %u )\n", stream, var_mode,
                      var_count, var_type, var_offset);
                this->glDrawElementsOffset(this, var_mode, var_count, var_type, var_offset);
                SET_LASTCALL("glDrawElementsOffset");
                break;
            }
            case OP_glDrawElementsData: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                GLsizei var_count = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_data __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                InputBuffer inptr_data(ptr + 8 + 4 + 4 + 4 + 4, size_data);
                GLuint var_datalen = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + size_data);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + size_data + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + size_data +
                                                             4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDrawElementsData: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDrawElementsData\n",
                            err);
#endif
                DEBUG("gles1(%p): glDrawElementsData(0x%08x %d 0x%08x %p(%u) %u )\n", stream,
                      var_mode, var_count, var_type, (void *) (inptr_data.get()), size_data,
                      var_datalen);
                this->glDrawElementsData(this, var_mode, var_count, var_type,
                                         (void *) (inptr_data.get()), var_datalen);
                SET_LASTCALL("glDrawElementsData");
                break;
            }
            case OP_glGetCompressedTextureFormats: {
                int var_count = Unpack<int, uint32_t>(ptr + 8);
                uint32_t size_formats __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetCompressedTextureFormats: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_formats;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_formats(&tmpBuf[0], size_formats);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glGetCompressedTextureFormats\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetCompressedTextureFormats(%d %p(%u) )\n", stream, var_count,
                      (GLint *) (outptr_formats.get()), size_formats);
                this->glGetCompressedTextureFormats(this, var_count,
                                                    (GLint *) (outptr_formats.get()));
                outptr_formats.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetCompressedTextureFormats");
                break;
            }
            case OP_glFinishRoundTrip: {
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8, ptr + 8,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFinishRoundTrip: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(int);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFinishRoundTrip\n", err);
#endif
                DEBUG("gles1(%p): glFinishRoundTrip()\n", stream);
                *(int *) (&tmpBuf[0]) = this->glFinishRoundTrip(this);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glFinishRoundTrip");
                break;
            }
            case OP_glBlendEquationSeparateOES: {
                GLenum var_modeRGB = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_modeAlpha = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glBlendEquationSeparateOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glBlendEquationSeparateOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glBlendEquationSeparateOES(0x%08x 0x%08x )\n", stream,
                      var_modeRGB, var_modeAlpha);
                this->glBlendEquationSeparateOES(var_modeRGB, var_modeAlpha);
                SET_LASTCALL("glBlendEquationSeparateOES");
                break;
            }
            case OP_glBlendFuncSeparateOES: {
                GLenum var_srcRGB = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_dstRGB = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLenum var_srcAlpha = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                GLenum var_dstAlpha = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glBlendFuncSeparateOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glBlendFuncSeparateOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glBlendFuncSeparateOES(0x%08x 0x%08x 0x%08x 0x%08x )\n", stream,
                      var_srcRGB, var_dstRGB, var_srcAlpha, var_dstAlpha);
                this->glBlendFuncSeparateOES(var_srcRGB, var_dstRGB, var_srcAlpha, var_dstAlpha);
                SET_LASTCALL("glBlendFuncSeparateOES");
                break;
            }
            case OP_glBlendEquationOES: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glBlendEquationOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glBlendEquationOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glBlendEquationOES(0x%08x )\n", stream, var_mode);
                this->glBlendEquationOES(var_mode);
                SET_LASTCALL("glBlendEquationOES");
                break;
            }
            case OP_glDrawTexsOES: {
                GLshort var_x = Unpack<GLshort, uint16_t>(ptr + 8);
                GLshort var_y = Unpack<GLshort, uint16_t>(ptr + 8 + 2);
                GLshort var_z = Unpack<GLshort, uint16_t>(ptr + 8 + 2 + 2);
                GLshort var_width = Unpack<GLshort, uint16_t>(ptr + 8 + 2 + 2 + 2);
                GLshort var_height = Unpack<GLshort, uint16_t>(ptr + 8 + 2 + 2 + 2 + 2);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 2 + 2 + 2 + 2 + 2,
                                                             ptr + 8 + 2 + 2 + 2 + 2 + 2,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDrawTexsOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDrawTexsOES\n", err);
#endif
                DEBUG("gles1(%p): glDrawTexsOES(%d %d %d %d %d )\n", stream, var_x, var_y, var_z,
                      var_width, var_height);
                this->glDrawTexsOES(var_x, var_y, var_z, var_width, var_height);
                SET_LASTCALL("glDrawTexsOES");
                break;
            }
            case OP_glDrawTexiOES: {
                GLint var_x = Unpack<GLint, uint32_t>(ptr + 8);
                GLint var_y = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLint var_z = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4);
                GLint var_width = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLint var_height = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDrawTexiOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDrawTexiOES\n", err);
#endif
                DEBUG("gles1(%p): glDrawTexiOES(%d %d %d %d %d )\n", stream, var_x, var_y, var_z,
                      var_width, var_height);
                this->glDrawTexiOES(var_x, var_y, var_z, var_width, var_height);
                SET_LASTCALL("glDrawTexiOES");
                break;
            }
            case OP_glDrawTexxOES: {
                GLfixed var_x = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_y = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_z = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                GLfixed var_width = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfixed var_height = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDrawTexxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDrawTexxOES\n", err);
#endif
                DEBUG("gles1(%p): glDrawTexxOES(0x%08x 0x%08x 0x%08x 0x%08x 0x%08x )\n", stream,
                      var_x, var_y, var_z, var_width, var_height);
                this->glDrawTexxOES(var_x, var_y, var_z, var_width, var_height);
                SET_LASTCALL("glDrawTexxOES");
                break;
            }
            case OP_glDrawTexsvOES: {
                uint32_t size_coords __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                InputBuffer inptr_coords(ptr + 8 + 4, size_coords);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + size_coords,
                                                             ptr + 8 + 4 + size_coords,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDrawTexsvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDrawTexsvOES\n", err);
#endif
                DEBUG("gles1(%p): glDrawTexsvOES(%p(%u) )\n", stream,
                      (const GLshort *) (inptr_coords.get()), size_coords);
                this->glDrawTexsvOES((const GLshort *) (inptr_coords.get()));
                SET_LASTCALL("glDrawTexsvOES");
                break;
            }
            case OP_glDrawTexivOES: {
                uint32_t size_coords __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                InputBuffer inptr_coords(ptr + 8 + 4, size_coords);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + size_coords,
                                                             ptr + 8 + 4 + size_coords,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDrawTexivOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDrawTexivOES\n", err);
#endif
                DEBUG("gles1(%p): glDrawTexivOES(%p(%u) )\n", stream,
                      (const GLint *) (inptr_coords.get()), size_coords);
                this->glDrawTexivOES((const GLint *) (inptr_coords.get()));
                SET_LASTCALL("glDrawTexivOES");
                break;
            }
            case OP_glDrawTexxvOES: {
                uint32_t size_coords __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                InputBuffer inptr_coords(ptr + 8 + 4, size_coords);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + size_coords,
                                                             ptr + 8 + 4 + size_coords,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDrawTexxvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDrawTexxvOES\n", err);
#endif
                DEBUG("gles1(%p): glDrawTexxvOES(%p(%u) )\n", stream,
                      (const GLfixed *) (inptr_coords.get()), size_coords);
                this->glDrawTexxvOES((const GLfixed *) (inptr_coords.get()));
                SET_LASTCALL("glDrawTexxvOES");
                break;
            }
            case OP_glDrawTexfOES: {
                GLfloat var_x = Unpack<GLfloat, uint32_t>(ptr + 8);
                GLfloat var_y = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                GLfloat var_z = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                GLfloat var_width = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfloat var_height = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDrawTexfOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDrawTexfOES\n", err);
#endif
                DEBUG("gles1(%p): glDrawTexfOES(%f %f %f %f %f )\n", stream, var_x, var_y, var_z,
                      var_width, var_height);
                this->glDrawTexfOES(var_x, var_y, var_z, var_width, var_height);
                SET_LASTCALL("glDrawTexfOES");
                break;
            }
            case OP_glDrawTexfvOES: {
                uint32_t size_coords __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                InputBuffer inptr_coords(ptr + 8 + 4, size_coords);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + size_coords,
                                                             ptr + 8 + 4 + size_coords,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDrawTexfvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDrawTexfvOES\n", err);
#endif
                DEBUG("gles1(%p): glDrawTexfvOES(%p(%u) )\n", stream,
                      (const GLfloat *) (inptr_coords.get()), size_coords);
                this->glDrawTexfvOES((const GLfloat *) (inptr_coords.get()));
                SET_LASTCALL("glDrawTexfvOES");
                break;
            }
            case OP_glEGLImageTargetTexture2DOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLeglImageOES var_image = Unpack<GLeglImageOES, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glEGLImageTargetTexture2DOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glEGLImageTargetTexture2DOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glEGLImageTargetTexture2DOES(0x%08x %p )\n", stream, var_target,
                      var_image);
                this->glEGLImageTargetTexture2DOES(var_target, var_image);
                SET_LASTCALL("glEGLImageTargetTexture2DOES");
                break;
            }
            case OP_glEGLImageTargetRenderbufferStorageOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLeglImageOES var_image = Unpack<GLeglImageOES, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glEGLImageTargetRenderbufferStorageOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glEGLImageTargetRenderbufferStorageOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glEGLImageTargetRenderbufferStorageOES(0x%08x %p )\n", stream,
                      var_target, var_image);
                this->glEGLImageTargetRenderbufferStorageOES(var_target, var_image);
                SET_LASTCALL("glEGLImageTargetRenderbufferStorageOES");
                break;
            }
            case OP_glAlphaFuncxOES: {
                GLenum var_func = Unpack<GLenum, uint32_t>(ptr + 8);
                GLclampx var_ref = Unpack<GLclampx, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glAlphaFuncxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glAlphaFuncxOES\n", err);
#endif
                DEBUG("gles1(%p): glAlphaFuncxOES(0x%08x 0x%08x )\n", stream, var_func, var_ref);
                this->glAlphaFuncxOES(var_func, var_ref);
                SET_LASTCALL("glAlphaFuncxOES");
                break;
            }
            case OP_glClearColorxOES: {
                GLclampx var_red = Unpack<GLclampx, uint32_t>(ptr + 8);
                GLclampx var_green = Unpack<GLclampx, uint32_t>(ptr + 8 + 4);
                GLclampx var_blue = Unpack<GLclampx, uint32_t>(ptr + 8 + 4 + 4);
                GLclampx var_alpha = Unpack<GLclampx, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClearColorxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClearColorxOES\n", err);
#endif
                DEBUG("gles1(%p): glClearColorxOES(0x%08x 0x%08x 0x%08x 0x%08x )\n", stream,
                      var_red, var_green, var_blue, var_alpha);
                this->glClearColorxOES(var_red, var_green, var_blue, var_alpha);
                SET_LASTCALL("glClearColorxOES");
                break;
            }
            case OP_glClearDepthxOES: {
                GLclampx var_depth = Unpack<GLclampx, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClearDepthxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClearDepthxOES\n", err);
#endif
                DEBUG("gles1(%p): glClearDepthxOES(0x%08x )\n", stream, var_depth);
                this->glClearDepthxOES(var_depth);
                SET_LASTCALL("glClearDepthxOES");
                break;
            }
            case OP_glClipPlanexOES: {
                GLenum var_plane = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_equation __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_equation(ptr + 8 + 4 + 4, size_equation);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_equation,
                                                             ptr + 8 + 4 + 4 + size_equation,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClipPlanexOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClipPlanexOES\n", err);
#endif
                DEBUG("gles1(%p): glClipPlanexOES(0x%08x %p(%u) )\n", stream, var_plane,
                      (const GLfixed *) (inptr_equation.get()), size_equation);
                this->glClipPlanexOES(var_plane, (const GLfixed *) (inptr_equation.get()));
                SET_LASTCALL("glClipPlanexOES");
                break;
            }
            case OP_glClipPlanexIMG: {
                GLenum var_plane = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_equation __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_equation(ptr + 8 + 4 + 4, size_equation);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_equation,
                                                             ptr + 8 + 4 + 4 + size_equation,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClipPlanexIMG: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClipPlanexIMG\n", err);
#endif
                DEBUG("gles1(%p): glClipPlanexIMG(0x%08x %p(%u) )\n", stream, var_plane,
                      (const GLfixed *) (inptr_equation.get()), size_equation);
                this->glClipPlanexIMG(var_plane, (const GLfixed *) (inptr_equation.get()));
                SET_LASTCALL("glClipPlanexIMG");
                break;
            }
            case OP_glColor4xOES: {
                GLfixed var_red = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_green = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_blue = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                GLfixed var_alpha = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glColor4xOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glColor4xOES\n", err);
#endif
                DEBUG("gles1(%p): glColor4xOES(0x%08x 0x%08x 0x%08x 0x%08x )\n", stream, var_red,
                      var_green, var_blue, var_alpha);
                this->glColor4xOES(var_red, var_green, var_blue, var_alpha);
                SET_LASTCALL("glColor4xOES");
                break;
            }
            case OP_glDepthRangexOES: {
                GLclampx var_zNear = Unpack<GLclampx, uint32_t>(ptr + 8);
                GLclampx var_zFar = Unpack<GLclampx, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDepthRangexOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDepthRangexOES\n", err);
#endif
                DEBUG("gles1(%p): glDepthRangexOES(0x%08x 0x%08x )\n", stream, var_zNear, var_zFar);
                this->glDepthRangexOES(var_zNear, var_zFar);
                SET_LASTCALL("glDepthRangexOES");
                break;
            }
            case OP_glFogxOES: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFogxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFogxOES\n", err);
#endif
                DEBUG("gles1(%p): glFogxOES(0x%08x 0x%08x )\n", stream, var_pname, var_param);
                this->glFogxOES(var_pname, var_param);
                SET_LASTCALL("glFogxOES");
                break;
            }
            case OP_glFogxvOES: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFogxvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFogxvOES\n", err);
#endif
                DEBUG("gles1(%p): glFogxvOES(0x%08x %p(%u) )\n", stream, var_pname,
                      (const GLfixed *) (inptr_params.get()), size_params);
                this->glFogxvOES(var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glFogxvOES");
                break;
            }
            case OP_glFrustumxOES: {
                GLfixed var_left = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_right = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_bottom = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                GLfixed var_top = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfixed var_zNear = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLfixed var_zFar = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFrustumxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFrustumxOES\n", err);
#endif
                DEBUG("gles1(%p): glFrustumxOES(0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x )\n",
                      stream, var_left, var_right, var_bottom, var_top, var_zNear, var_zFar);
                this->glFrustumxOES(var_left, var_right, var_bottom, var_top, var_zNear, var_zFar);
                SET_LASTCALL("glFrustumxOES");
                break;
            }
            case OP_glGetClipPlanexOES: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_eqn __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetClipPlanexOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_eqn;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_eqn(&tmpBuf[0], size_eqn);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetClipPlanexOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetClipPlanexOES(0x%08x %p(%u) )\n", stream, var_pname,
                      (GLfixed *) (outptr_eqn.get()), size_eqn);
                this->glGetClipPlanexOES(var_pname, (GLfixed *) (outptr_eqn.get()));
                outptr_eqn.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetClipPlanexOES");
                break;
            }
            case OP_glGetClipPlanex: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_eqn __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetClipPlanex: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_eqn;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_eqn(&tmpBuf[0], size_eqn);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetClipPlanex\n", err);
#endif
                DEBUG("gles1(%p): glGetClipPlanex(0x%08x %p(%u) )\n", stream, var_pname,
                      (GLfixed *) (outptr_eqn.get()), size_eqn);
                this->glGetClipPlanex(var_pname, (GLfixed *) (outptr_eqn.get()));
                outptr_eqn.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetClipPlanex");
                break;
            }
            case OP_glGetFixedvOES: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetFixedvOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetFixedvOES\n", err);
#endif
                DEBUG("gles1(%p): glGetFixedvOES(0x%08x %p(%u) )\n", stream, var_pname,
                      (GLfixed *) (outptr_params.get()), size_params);
                this->glGetFixedvOES(var_pname, (GLfixed *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetFixedvOES");
                break;
            }
            case OP_glGetLightxvOES: {
                GLenum var_light = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetLightxvOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetLightxvOES\n", err);
#endif
                DEBUG("gles1(%p): glGetLightxvOES(0x%08x 0x%08x %p(%u) )\n", stream, var_light,
                      var_pname, (GLfixed *) (outptr_params.get()), size_params);
                this->glGetLightxvOES(var_light, var_pname, (GLfixed *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetLightxvOES");
                break;
            }
            case OP_glGetMaterialxvOES: {
                GLenum var_face = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetMaterialxvOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetMaterialxvOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetMaterialxvOES(0x%08x 0x%08x %p(%u) )\n", stream, var_face,
                      var_pname, (GLfixed *) (outptr_params.get()), size_params);
                this->glGetMaterialxvOES(var_face, var_pname, (GLfixed *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetMaterialxvOES");
                break;
            }
            case OP_glGetTexEnvxvOES: {
                GLenum var_env = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetTexEnvxvOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetTexEnvxvOES\n", err);
#endif
                DEBUG("gles1(%p): glGetTexEnvxvOES(0x%08x 0x%08x %p(%u) )\n", stream, var_env,
                      var_pname, (GLfixed *) (outptr_params.get()), size_params);
                this->glGetTexEnvxvOES(var_env, var_pname, (GLfixed *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetTexEnvxvOES");
                break;
            }
            case OP_glGetTexParameterxvOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetTexParameterxvOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetTexParameterxvOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetTexParameterxvOES(0x%08x 0x%08x %p(%u) )\n", stream,
                      var_target, var_pname, (GLfixed *) (outptr_params.get()), size_params);
                this->glGetTexParameterxvOES(var_target, var_pname,
                                             (GLfixed *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetTexParameterxvOES");
                break;
            }
            case OP_glLightModelxOES: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLightModelxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLightModelxOES\n", err);
#endif
                DEBUG("gles1(%p): glLightModelxOES(0x%08x 0x%08x )\n", stream, var_pname,
                      var_param);
                this->glLightModelxOES(var_pname, var_param);
                SET_LASTCALL("glLightModelxOES");
                break;
            }
            case OP_glLightModelxvOES: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLightModelxvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLightModelxvOES\n", err);
#endif
                DEBUG("gles1(%p): glLightModelxvOES(0x%08x %p(%u) )\n", stream, var_pname,
                      (const GLfixed *) (inptr_params.get()), size_params);
                this->glLightModelxvOES(var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glLightModelxvOES");
                break;
            }
            case OP_glLightxOES: {
                GLenum var_light = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLightxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLightxOES\n", err);
#endif
                DEBUG("gles1(%p): glLightxOES(0x%08x 0x%08x 0x%08x )\n", stream, var_light,
                      var_pname, var_param);
                this->glLightxOES(var_light, var_pname, var_param);
                SET_LASTCALL("glLightxOES");
                break;
            }
            case OP_glLightxvOES: {
                GLenum var_light = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLightxvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLightxvOES\n", err);
#endif
                DEBUG("gles1(%p): glLightxvOES(0x%08x 0x%08x %p(%u) )\n", stream, var_light,
                      var_pname, (const GLfixed *) (inptr_params.get()), size_params);
                this->glLightxvOES(var_light, var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glLightxvOES");
                break;
            }
            case OP_glLineWidthxOES: {
                GLfixed var_width = Unpack<GLfixed, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLineWidthxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLineWidthxOES\n", err);
#endif
                DEBUG("gles1(%p): glLineWidthxOES(0x%08x )\n", stream, var_width);
                this->glLineWidthxOES(var_width);
                SET_LASTCALL("glLineWidthxOES");
                break;
            }
            case OP_glLoadMatrixxOES: {
                uint32_t size_m __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                InputBuffer inptr_m(ptr + 8 + 4, size_m);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + size_m,
                                                             ptr + 8 + 4 + size_m, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLoadMatrixxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glLoadMatrixxOES\n", err);
#endif
                DEBUG("gles1(%p): glLoadMatrixxOES(%p(%u) )\n", stream,
                      (const GLfixed *) (inptr_m.get()), size_m);
                this->glLoadMatrixxOES((const GLfixed *) (inptr_m.get()));
                SET_LASTCALL("glLoadMatrixxOES");
                break;
            }
            case OP_glMaterialxOES: {
                GLenum var_face = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMaterialxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMaterialxOES\n", err);
#endif
                DEBUG("gles1(%p): glMaterialxOES(0x%08x 0x%08x 0x%08x )\n", stream, var_face,
                      var_pname, var_param);
                this->glMaterialxOES(var_face, var_pname, var_param);
                SET_LASTCALL("glMaterialxOES");
                break;
            }
            case OP_glMaterialxvOES: {
                GLenum var_face = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMaterialxvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMaterialxvOES\n", err);
#endif
                DEBUG("gles1(%p): glMaterialxvOES(0x%08x 0x%08x %p(%u) )\n", stream, var_face,
                      var_pname, (const GLfixed *) (inptr_params.get()), size_params);
                this->glMaterialxvOES(var_face, var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glMaterialxvOES");
                break;
            }
            case OP_glMultMatrixxOES: {
                uint32_t size_m __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                InputBuffer inptr_m(ptr + 8 + 4, size_m);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + size_m,
                                                             ptr + 8 + 4 + size_m, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMultMatrixxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMultMatrixxOES\n", err);
#endif
                DEBUG("gles1(%p): glMultMatrixxOES(%p(%u) )\n", stream,
                      (const GLfixed *) (inptr_m.get()), size_m);
                this->glMultMatrixxOES((const GLfixed *) (inptr_m.get()));
                SET_LASTCALL("glMultMatrixxOES");
                break;
            }
            case OP_glMultiTexCoord4xOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLfixed var_s = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_t = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                GLfixed var_r = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfixed var_q = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMultiTexCoord4xOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMultiTexCoord4xOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glMultiTexCoord4xOES(0x%08x 0x%08x 0x%08x 0x%08x 0x%08x )\n",
                      stream, var_target, var_s, var_t, var_r, var_q);
                this->glMultiTexCoord4xOES(var_target, var_s, var_t, var_r, var_q);
                SET_LASTCALL("glMultiTexCoord4xOES");
                break;
            }
            case OP_glNormal3xOES: {
                GLfixed var_nx = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_ny = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_nz = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glNormal3xOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glNormal3xOES\n", err);
#endif
                DEBUG("gles1(%p): glNormal3xOES(0x%08x 0x%08x 0x%08x )\n", stream, var_nx, var_ny,
                      var_nz);
                this->glNormal3xOES(var_nx, var_ny, var_nz);
                SET_LASTCALL("glNormal3xOES");
                break;
            }
            case OP_glOrthoxOES: {
                GLfixed var_left = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_right = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_bottom = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                GLfixed var_top = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfixed var_zNear = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLfixed var_zFar = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glOrthoxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glOrthoxOES\n", err);
#endif
                DEBUG("gles1(%p): glOrthoxOES(0x%08x 0x%08x 0x%08x 0x%08x 0x%08x 0x%08x )\n",
                      stream, var_left, var_right, var_bottom, var_top, var_zNear, var_zFar);
                this->glOrthoxOES(var_left, var_right, var_bottom, var_top, var_zNear, var_zFar);
                SET_LASTCALL("glOrthoxOES");
                break;
            }
            case OP_glPointParameterxOES: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPointParameterxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPointParameterxOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glPointParameterxOES(0x%08x 0x%08x )\n", stream, var_pname,
                      var_param);
                this->glPointParameterxOES(var_pname, var_param);
                SET_LASTCALL("glPointParameterxOES");
                break;
            }
            case OP_glPointParameterxvOES: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPointParameterxvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPointParameterxvOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glPointParameterxvOES(0x%08x %p(%u) )\n", stream, var_pname,
                      (const GLfixed *) (inptr_params.get()), size_params);
                this->glPointParameterxvOES(var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glPointParameterxvOES");
                break;
            }
            case OP_glPointSizexOES: {
                GLfixed var_size = Unpack<GLfixed, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPointSizexOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPointSizexOES\n", err);
#endif
                DEBUG("gles1(%p): glPointSizexOES(0x%08x )\n", stream, var_size);
                this->glPointSizexOES(var_size);
                SET_LASTCALL("glPointSizexOES");
                break;
            }
            case OP_glPolygonOffsetxOES: {
                GLfixed var_factor = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_units = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glPolygonOffsetxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glPolygonOffsetxOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glPolygonOffsetxOES(0x%08x 0x%08x )\n", stream, var_factor,
                      var_units);
                this->glPolygonOffsetxOES(var_factor, var_units);
                SET_LASTCALL("glPolygonOffsetxOES");
                break;
            }
            case OP_glRotatexOES: {
                GLfixed var_angle = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_x = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_y = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                GLfixed var_z = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glRotatexOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glRotatexOES\n", err);
#endif
                DEBUG("gles1(%p): glRotatexOES(0x%08x 0x%08x 0x%08x 0x%08x )\n", stream, var_angle,
                      var_x, var_y, var_z);
                this->glRotatexOES(var_angle, var_x, var_y, var_z);
                SET_LASTCALL("glRotatexOES");
                break;
            }
            case OP_glSampleCoveragexOES: {
                GLclampx var_value = Unpack<GLclampx, uint32_t>(ptr + 8);
                GLboolean var_invert = Unpack<GLboolean, uint8_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 1,
                                                             ptr + 8 + 4 + 1, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glSampleCoveragexOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glSampleCoveragexOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glSampleCoveragexOES(0x%08x %d )\n", stream, var_value,
                      var_invert);
                this->glSampleCoveragexOES(var_value, var_invert);
                SET_LASTCALL("glSampleCoveragexOES");
                break;
            }
            case OP_glScalexOES: {
                GLfixed var_x = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_y = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_z = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glScalexOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glScalexOES\n", err);
#endif
                DEBUG("gles1(%p): glScalexOES(0x%08x 0x%08x 0x%08x )\n", stream, var_x, var_y,
                      var_z);
                this->glScalexOES(var_x, var_y, var_z);
                SET_LASTCALL("glScalexOES");
                break;
            }
            case OP_glTexEnvxOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexEnvxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexEnvxOES\n", err);
#endif
                DEBUG("gles1(%p): glTexEnvxOES(0x%08x 0x%08x 0x%08x )\n", stream, var_target,
                      var_pname, var_param);
                this->glTexEnvxOES(var_target, var_pname, var_param);
                SET_LASTCALL("glTexEnvxOES");
                break;
            }
            case OP_glTexEnvxvOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexEnvxvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexEnvxvOES\n", err);
#endif
                DEBUG("gles1(%p): glTexEnvxvOES(0x%08x 0x%08x %p(%u) )\n", stream, var_target,
                      var_pname, (const GLfixed *) (inptr_params.get()), size_params);
                this->glTexEnvxvOES(var_target, var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glTexEnvxvOES");
                break;
            }
            case OP_glTexParameterxOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexParameterxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexParameterxOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glTexParameterxOES(0x%08x 0x%08x 0x%08x )\n", stream, var_target,
                      var_pname, var_param);
                this->glTexParameterxOES(var_target, var_pname, var_param);
                SET_LASTCALL("glTexParameterxOES");
                break;
            }
            case OP_glTexParameterxvOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexParameterxvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexParameterxvOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glTexParameterxvOES(0x%08x 0x%08x %p(%u) )\n", stream, var_target,
                      var_pname, (const GLfixed *) (inptr_params.get()), size_params);
                this->glTexParameterxvOES(var_target, var_pname,
                                          (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glTexParameterxvOES");
                break;
            }
            case OP_glTranslatexOES: {
                GLfixed var_x = Unpack<GLfixed, uint32_t>(ptr + 8);
                GLfixed var_y = Unpack<GLfixed, uint32_t>(ptr + 8 + 4);
                GLfixed var_z = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTranslatexOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTranslatexOES\n", err);
#endif
                DEBUG("gles1(%p): glTranslatexOES(0x%08x 0x%08x 0x%08x )\n", stream, var_x, var_y,
                      var_z);
                this->glTranslatexOES(var_x, var_y, var_z);
                SET_LASTCALL("glTranslatexOES");
                break;
            }
            case OP_glIsRenderbufferOES: {
                GLuint var_renderbuffer = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glIsRenderbufferOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(GLboolean);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glIsRenderbufferOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glIsRenderbufferOES(%u )\n", stream, var_renderbuffer);
                *(GLboolean *) (&tmpBuf[0]) = this->glIsRenderbufferOES(var_renderbuffer);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glIsRenderbufferOES");
                break;
            }
            case OP_glBindRenderbufferOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLuint var_renderbuffer = Unpack<GLuint, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glBindRenderbufferOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glBindRenderbufferOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glBindRenderbufferOES(0x%08x %u )\n", stream, var_target,
                      var_renderbuffer);
                this->glBindRenderbufferOES(var_target, var_renderbuffer);
                SET_LASTCALL("glBindRenderbufferOES");
                break;
            }
            case OP_glDeleteRenderbuffersOES: {
                GLsizei var_n = Unpack<GLsizei, uint32_t>(ptr + 8);
                uint32_t size_renderbuffers __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_renderbuffers(ptr + 8 + 4 + 4, size_renderbuffers);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_renderbuffers,
                                                             ptr + 8 + 4 + 4 + size_renderbuffers,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDeleteRenderbuffersOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glDeleteRenderbuffersOES\n", err);
#endif
                DEBUG("gles1(%p): glDeleteRenderbuffersOES(%d %p(%u) )\n", stream, var_n,
                      (const GLuint *) (inptr_renderbuffers.get()), size_renderbuffers);
                this->glDeleteRenderbuffersOES_dec(this, var_n,
                                                   (const GLuint *) (inptr_renderbuffers.get()));
                SET_LASTCALL("glDeleteRenderbuffersOES");
                break;
            }
            case OP_glGenRenderbuffersOES: {
                GLsizei var_n = Unpack<GLsizei, uint32_t>(ptr + 8);
                uint32_t size_renderbuffers __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGenRenderbuffersOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_renderbuffers;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_renderbuffers(&tmpBuf[0], size_renderbuffers);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGenRenderbuffersOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glGenRenderbuffersOES(%d %p(%u) )\n", stream, var_n,
                      (GLuint *) (outptr_renderbuffers.get()), size_renderbuffers);
                this->glGenRenderbuffersOES_dec(this, var_n,
                                                (GLuint *) (outptr_renderbuffers.get()));
                outptr_renderbuffers.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGenRenderbuffersOES");
                break;
            }
            case OP_glRenderbufferStorageOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_internalformat = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_width = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                GLsizei var_height = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glRenderbufferStorageOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glRenderbufferStorageOES\n", err);
#endif
                DEBUG("gles1(%p): glRenderbufferStorageOES(0x%08x 0x%08x %d %d )\n", stream,
                      var_target, var_internalformat, var_width, var_height);
                this->glRenderbufferStorageOES(var_target, var_internalformat, var_width,
                                               var_height);
                SET_LASTCALL("glRenderbufferStorageOES");
                break;
            }
            case OP_glGetRenderbufferParameterivOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetRenderbufferParameterivOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glGetRenderbufferParameterivOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetRenderbufferParameterivOES(0x%08x 0x%08x %p(%u) )\n", stream,
                      var_target, var_pname, (GLint *) (outptr_params.get()), size_params);
                this->glGetRenderbufferParameterivOES(var_target, var_pname,
                                                      (GLint *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetRenderbufferParameterivOES");
                break;
            }
            case OP_glIsFramebufferOES: {
                GLuint var_framebuffer = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glIsFramebufferOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(GLboolean);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glIsFramebufferOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glIsFramebufferOES(%u )\n", stream, var_framebuffer);
                *(GLboolean *) (&tmpBuf[0]) = this->glIsFramebufferOES(var_framebuffer);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glIsFramebufferOES");
                break;
            }
            case OP_glBindFramebufferOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLuint var_framebuffer = Unpack<GLuint, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glBindFramebufferOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glBindFramebufferOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glBindFramebufferOES(0x%08x %u )\n", stream, var_target,
                      var_framebuffer);
                this->glBindFramebufferOES(var_target, var_framebuffer);
                SET_LASTCALL("glBindFramebufferOES");
                break;
            }
            case OP_glDeleteFramebuffersOES: {
                GLsizei var_n = Unpack<GLsizei, uint32_t>(ptr + 8);
                uint32_t size_framebuffers __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_framebuffers(ptr + 8 + 4 + 4, size_framebuffers);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_framebuffers,
                                                             ptr + 8 + 4 + 4 + size_framebuffers,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDeleteFramebuffersOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDeleteFramebuffersOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glDeleteFramebuffersOES(%d %p(%u) )\n", stream, var_n,
                      (const GLuint *) (inptr_framebuffers.get()), size_framebuffers);
                this->glDeleteFramebuffersOES_dec(this, var_n,
                                                  (const GLuint *) (inptr_framebuffers.get()));
                SET_LASTCALL("glDeleteFramebuffersOES");
                break;
            }
            case OP_glGenFramebuffersOES: {
                GLsizei var_n = Unpack<GLsizei, uint32_t>(ptr + 8);
                uint32_t size_framebuffers __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGenFramebuffersOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_framebuffers;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_framebuffers(&tmpBuf[0], size_framebuffers);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGenFramebuffersOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glGenFramebuffersOES(%d %p(%u) )\n", stream, var_n,
                      (GLuint *) (outptr_framebuffers.get()), size_framebuffers);
                this->glGenFramebuffersOES_dec(this, var_n, (GLuint *) (outptr_framebuffers.get()));
                outptr_framebuffers.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGenFramebuffersOES");
                break;
            }
            case OP_glCheckFramebufferStatusOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glCheckFramebufferStatusOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(GLenum);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glCheckFramebufferStatusOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glCheckFramebufferStatusOES(0x%08x )\n", stream, var_target);
                *(GLenum *) (&tmpBuf[0]) = this->glCheckFramebufferStatusOES(var_target);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glCheckFramebufferStatusOES");
                break;
            }
            case OP_glFramebufferRenderbufferOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_attachment = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLenum var_renderbuffertarget = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                GLuint var_renderbuffer = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFramebufferRenderbufferOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glFramebufferRenderbufferOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glFramebufferRenderbufferOES(0x%08x 0x%08x 0x%08x %u )\n", stream,
                      var_target, var_attachment, var_renderbuffertarget, var_renderbuffer);
                this->glFramebufferRenderbufferOES(var_target, var_attachment,
                                                   var_renderbuffertarget, var_renderbuffer);
                SET_LASTCALL("glFramebufferRenderbufferOES");
                break;
            }
            case OP_glFramebufferTexture2DOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_attachment = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLenum var_textarget = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                GLuint var_texture = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLint var_level = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFramebufferTexture2DOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glFramebufferTexture2DOES\n", err);
#endif
                DEBUG("gles1(%p): glFramebufferTexture2DOES(0x%08x 0x%08x 0x%08x %u %d )\n", stream,
                      var_target, var_attachment, var_textarget, var_texture, var_level);
                this->glFramebufferTexture2DOES(var_target, var_attachment, var_textarget,
                                                var_texture, var_level);
                SET_LASTCALL("glFramebufferTexture2DOES");
                break;
            }
            case OP_glGetFramebufferAttachmentParameterivOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_attachment = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetFramebufferAttachmentParameterivOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glGetFramebufferAttachmentParameterivOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetFramebufferAttachmentParameterivOES(0x%08x 0x%08x 0x%08x %p(%u) )\n",
                      stream, var_target, var_attachment, var_pname,
                      (GLint *) (outptr_params.get()), size_params);
                this->glGetFramebufferAttachmentParameterivOES(var_target, var_attachment,
                                                               var_pname,
                                                               (GLint *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetFramebufferAttachmentParameterivOES");
                break;
            }
            case OP_glGenerateMipmapOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGenerateMipmapOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGenerateMipmapOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glGenerateMipmapOES(0x%08x )\n", stream, var_target);
                this->glGenerateMipmapOES(var_target);
                SET_LASTCALL("glGenerateMipmapOES");
                break;
            }
            case OP_glMapBufferOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_access = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMapBufferOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMapBufferOES\n", err);
#endif
                DEBUG("gles1(%p): glMapBufferOES(0x%08x 0x%08x )\n", stream, var_target,
                      var_access);
                this->glMapBufferOES(var_target, var_access);
                SET_LASTCALL("glMapBufferOES");
                break;
            }
            case OP_glUnmapBufferOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glUnmapBufferOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(GLboolean);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glUnmapBufferOES\n", err);
#endif
                DEBUG("gles1(%p): glUnmapBufferOES(0x%08x )\n", stream, var_target);
                *(GLboolean *) (&tmpBuf[0]) = this->glUnmapBufferOES(var_target);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glUnmapBufferOES");
                break;
            }
            case OP_glGetBufferPointervOES: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetBufferPointervOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetBufferPointervOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetBufferPointervOES(0x%08x 0x%08x %p(%u) )\n", stream,
                      var_target, var_pname, (GLvoid **) (inptr_params.get()), size_params);
                this->glGetBufferPointervOES(var_target, var_pname,
                                             (GLvoid **) (inptr_params.get()));
                SET_LASTCALL("glGetBufferPointervOES");
                break;
            }
            case OP_glCurrentPaletteMatrixOES: {
                GLuint var_matrixpaletteindex = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glCurrentPaletteMatrixOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glCurrentPaletteMatrixOES\n", err);
#endif
                DEBUG("gles1(%p): glCurrentPaletteMatrixOES(%u )\n", stream,
                      var_matrixpaletteindex);
                this->glCurrentPaletteMatrixOES(var_matrixpaletteindex);
                SET_LASTCALL("glCurrentPaletteMatrixOES");
                break;
            }
            case OP_glLoadPaletteFromModelViewMatrixOES: {
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8, ptr + 8,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glLoadPaletteFromModelViewMatrixOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glLoadPaletteFromModelViewMatrixOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glLoadPaletteFromModelViewMatrixOES()\n", stream);
                this->glLoadPaletteFromModelViewMatrixOES();
                SET_LASTCALL("glLoadPaletteFromModelViewMatrixOES");
                break;
            }
            case OP_glMatrixIndexPointerOES: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_pointer __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                InputBuffer inptr_pointer(ptr + 8 + 4 + 4 + 4 + 4, size_pointer);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + size_pointer,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + size_pointer,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMatrixIndexPointerOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMatrixIndexPointerOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glMatrixIndexPointerOES(%d 0x%08x %d %p(%u) )\n", stream,
                      var_size, var_type, var_stride, (const GLvoid *) (inptr_pointer.get()),
                      size_pointer);
                this->glMatrixIndexPointerOES(var_size, var_type, var_stride,
                                              (const GLvoid *) (inptr_pointer.get()));
                SET_LASTCALL("glMatrixIndexPointerOES");
                break;
            }
            case OP_glWeightPointerOES: {
                GLint var_size = Unpack<GLint, uint32_t>(ptr + 8);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLsizei var_stride = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4);
                uint32_t size_pointer __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                InputBuffer inptr_pointer(ptr + 8 + 4 + 4 + 4 + 4, size_pointer);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + size_pointer,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + size_pointer,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glWeightPointerOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glWeightPointerOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glWeightPointerOES(%d 0x%08x %d %p(%u) )\n", stream, var_size,
                      var_type, var_stride, (const GLvoid *) (inptr_pointer.get()), size_pointer);
                this->glWeightPointerOES(var_size, var_type, var_stride,
                                         (const GLvoid *) (inptr_pointer.get()));
                SET_LASTCALL("glWeightPointerOES");
                break;
            }
            case OP_glQueryMatrixxOES: {
                uint32_t size_mantissa __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8);
                uint32_t size_exponent __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glQueryMatrixxOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_mantissa;
                totalTmpSize += size_exponent;
                totalTmpSize += sizeof(GLbitfield);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_mantissa(&tmpBuf[0], size_mantissa);
                OutputBuffer outptr_exponent(&tmpBuf[0 + size_mantissa], size_exponent);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glQueryMatrixxOES\n", err);
#endif
                DEBUG("gles1(%p): glQueryMatrixxOES(%p(%u) %p(%u) )\n", stream,
                      (GLfixed *) (outptr_mantissa.get()), size_mantissa,
                      (GLint *) (outptr_exponent.get()), size_exponent);
                *(GLbitfield *) (&tmpBuf[0 + size_mantissa +
                                         size_exponent]) = this->glQueryMatrixxOES(
                        (GLfixed *) (outptr_mantissa.get()), (GLint *) (outptr_exponent.get()));
                outptr_mantissa.flush();
                outptr_exponent.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glQueryMatrixxOES");
                break;
            }
            case OP_glDepthRangefOES: {
                GLclampf var_zNear = Unpack<GLclampf, uint32_t>(ptr + 8);
                GLclampf var_zFar = Unpack<GLclampf, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDepthRangefOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDepthRangefOES\n", err);
#endif
                DEBUG("gles1(%p): glDepthRangefOES(%f %f )\n", stream, var_zNear, var_zFar);
                this->glDepthRangefOES(var_zNear, var_zFar);
                SET_LASTCALL("glDepthRangefOES");
                break;
            }
            case OP_glFrustumfOES: {
                GLfloat var_left = Unpack<GLfloat, uint32_t>(ptr + 8);
                GLfloat var_right = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                GLfloat var_bottom = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                GLfloat var_top = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfloat var_zNear = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLfloat var_zFar = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFrustumfOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFrustumfOES\n", err);
#endif
                DEBUG("gles1(%p): glFrustumfOES(%f %f %f %f %f %f )\n", stream, var_left, var_right,
                      var_bottom, var_top, var_zNear, var_zFar);
                this->glFrustumfOES(var_left, var_right, var_bottom, var_top, var_zNear, var_zFar);
                SET_LASTCALL("glFrustumfOES");
                break;
            }
            case OP_glOrthofOES: {
                GLfloat var_left = Unpack<GLfloat, uint32_t>(ptr + 8);
                GLfloat var_right = Unpack<GLfloat, uint32_t>(ptr + 8 + 4);
                GLfloat var_bottom = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                GLfloat var_top = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLfloat var_zNear = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLfloat var_zFar = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glOrthofOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glOrthofOES\n", err);
#endif
                DEBUG("gles1(%p): glOrthofOES(%f %f %f %f %f %f )\n", stream, var_left, var_right,
                      var_bottom, var_top, var_zNear, var_zFar);
                this->glOrthofOES(var_left, var_right, var_bottom, var_top, var_zNear, var_zFar);
                SET_LASTCALL("glOrthofOES");
                break;
            }
            case OP_glClipPlanefOES: {
                GLenum var_plane = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_equation __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_equation(ptr + 8 + 4 + 4, size_equation);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_equation,
                                                             ptr + 8 + 4 + 4 + size_equation,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClipPlanefOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClipPlanefOES\n", err);
#endif
                DEBUG("gles1(%p): glClipPlanefOES(0x%08x %p(%u) )\n", stream, var_plane,
                      (const GLfloat *) (inptr_equation.get()), size_equation);
                this->glClipPlanefOES(var_plane, (const GLfloat *) (inptr_equation.get()));
                SET_LASTCALL("glClipPlanefOES");
                break;
            }
            case OP_glClipPlanefIMG: {
                GLenum var_plane = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_equation __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_equation(ptr + 8 + 4 + 4, size_equation);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_equation,
                                                             ptr + 8 + 4 + 4 + size_equation,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClipPlanefIMG: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClipPlanefIMG\n", err);
#endif
                DEBUG("gles1(%p): glClipPlanefIMG(0x%08x %p(%u) )\n", stream, var_plane,
                      (const GLfloat *) (inptr_equation.get()), size_equation);
                this->glClipPlanefIMG(var_plane, (const GLfloat *) (inptr_equation.get()));
                SET_LASTCALL("glClipPlanefIMG");
                break;
            }
            case OP_glGetClipPlanefOES: {
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_eqn __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetClipPlanefOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_eqn;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_eqn(&tmpBuf[0], size_eqn);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetClipPlanefOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetClipPlanefOES(0x%08x %p(%u) )\n", stream, var_pname,
                      (GLfloat *) (outptr_eqn.get()), size_eqn);
                this->glGetClipPlanefOES(var_pname, (GLfloat *) (outptr_eqn.get()));
                outptr_eqn.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetClipPlanefOES");
                break;
            }
            case OP_glClearDepthfOES: {
                GLclampf var_depth = Unpack<GLclampf, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glClearDepthfOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glClearDepthfOES\n", err);
#endif
                DEBUG("gles1(%p): glClearDepthfOES(%f )\n", stream, var_depth);
                this->glClearDepthfOES(var_depth);
                SET_LASTCALL("glClearDepthfOES");
                break;
            }
            case OP_glTexGenfOES: {
                GLenum var_coord = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfloat var_param = Unpack<GLfloat, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexGenfOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexGenfOES\n", err);
#endif
                DEBUG("gles1(%p): glTexGenfOES(0x%08x 0x%08x %f )\n", stream, var_coord, var_pname,
                      var_param);
                this->glTexGenfOES(var_coord, var_pname, var_param);
                SET_LASTCALL("glTexGenfOES");
                break;
            }
            case OP_glTexGenfvOES: {
                GLenum var_coord = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexGenfvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexGenfvOES\n", err);
#endif
                DEBUG("gles1(%p): glTexGenfvOES(0x%08x 0x%08x %p(%u) )\n", stream, var_coord,
                      var_pname, (const GLfloat *) (inptr_params.get()), size_params);
                this->glTexGenfvOES(var_coord, var_pname, (const GLfloat *) (inptr_params.get()));
                SET_LASTCALL("glTexGenfvOES");
                break;
            }
            case OP_glTexGeniOES: {
                GLenum var_coord = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLint var_param = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexGeniOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexGeniOES\n", err);
#endif
                DEBUG("gles1(%p): glTexGeniOES(0x%08x 0x%08x %d )\n", stream, var_coord, var_pname,
                      var_param);
                this->glTexGeniOES(var_coord, var_pname, var_param);
                SET_LASTCALL("glTexGeniOES");
                break;
            }
            case OP_glTexGenivOES: {
                GLenum var_coord = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexGenivOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexGenivOES\n", err);
#endif
                DEBUG("gles1(%p): glTexGenivOES(0x%08x 0x%08x %p(%u) )\n", stream, var_coord,
                      var_pname, (const GLint *) (inptr_params.get()), size_params);
                this->glTexGenivOES(var_coord, var_pname, (const GLint *) (inptr_params.get()));
                SET_LASTCALL("glTexGenivOES");
                break;
            }
            case OP_glTexGenxOES: {
                GLenum var_coord = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLfixed var_param = Unpack<GLfixed, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexGenxOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexGenxOES\n", err);
#endif
                DEBUG("gles1(%p): glTexGenxOES(0x%08x 0x%08x 0x%08x )\n", stream, var_coord,
                      var_pname, var_param);
                this->glTexGenxOES(var_coord, var_pname, var_param);
                SET_LASTCALL("glTexGenxOES");
                break;
            }
            case OP_glTexGenxvOES: {
                GLenum var_coord = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTexGenxvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTexGenxvOES\n", err);
#endif
                DEBUG("gles1(%p): glTexGenxvOES(0x%08x 0x%08x %p(%u) )\n", stream, var_coord,
                      var_pname, (const GLfixed *) (inptr_params.get()), size_params);
                this->glTexGenxvOES(var_coord, var_pname, (const GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glTexGenxvOES");
                break;
            }
            case OP_glGetTexGenfvOES: {
                GLenum var_coord = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetTexGenfvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetTexGenfvOES\n", err);
#endif
                DEBUG("gles1(%p): glGetTexGenfvOES(0x%08x 0x%08x %p(%u) )\n", stream, var_coord,
                      var_pname, (GLfloat *) (inptr_params.get()), size_params);
                this->glGetTexGenfvOES(var_coord, var_pname, (GLfloat *) (inptr_params.get()));
                SET_LASTCALL("glGetTexGenfvOES");
                break;
            }
            case OP_glGetTexGenivOES: {
                GLenum var_coord = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetTexGenivOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetTexGenivOES\n", err);
#endif
                DEBUG("gles1(%p): glGetTexGenivOES(0x%08x 0x%08x %p(%u) )\n", stream, var_coord,
                      var_pname, (GLint *) (inptr_params.get()), size_params);
                this->glGetTexGenivOES(var_coord, var_pname, (GLint *) (inptr_params.get()));
                SET_LASTCALL("glGetTexGenivOES");
                break;
            }
            case OP_glGetTexGenxvOES: {
                GLenum var_coord = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetTexGenxvOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetTexGenxvOES\n", err);
#endif
                DEBUG("gles1(%p): glGetTexGenxvOES(0x%08x 0x%08x %p(%u) )\n", stream, var_coord,
                      var_pname, (GLfixed *) (inptr_params.get()), size_params);
                this->glGetTexGenxvOES(var_coord, var_pname, (GLfixed *) (inptr_params.get()));
                SET_LASTCALL("glGetTexGenxvOES");
                break;
            }
            case OP_glBindVertexArrayOES: {
                GLuint var_array = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glBindVertexArrayOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glBindVertexArrayOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glBindVertexArrayOES(%u )\n", stream, var_array);
                this->glBindVertexArrayOES(var_array);
                SET_LASTCALL("glBindVertexArrayOES");
                break;
            }
            case OP_glDeleteVertexArraysOES: {
                GLsizei var_n = Unpack<GLsizei, uint32_t>(ptr + 8);
                uint32_t size_arrays __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_arrays(ptr + 8 + 4 + 4, size_arrays);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_arrays,
                                                             ptr + 8 + 4 + 4 + size_arrays,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDeleteVertexArraysOES: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDeleteVertexArraysOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glDeleteVertexArraysOES(%d %p(%u) )\n", stream, var_n,
                      (const GLuint *) (inptr_arrays.get()), size_arrays);
                this->glDeleteVertexArraysOES_dec(this, var_n,
                                                  (const GLuint *) (inptr_arrays.get()));
                SET_LASTCALL("glDeleteVertexArraysOES");
                break;
            }
            case OP_glGenVertexArraysOES: {
                GLsizei var_n = Unpack<GLsizei, uint32_t>(ptr + 8);
                uint32_t size_arrays __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGenVertexArraysOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_arrays;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_arrays(&tmpBuf[0], size_arrays);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGenVertexArraysOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glGenVertexArraysOES(%d %p(%u) )\n", stream, var_n,
                      (GLuint *) (outptr_arrays.get()), size_arrays);
                this->glGenVertexArraysOES_dec(this, var_n, (GLuint *) (outptr_arrays.get()));
                outptr_arrays.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGenVertexArraysOES");
                break;
            }
            case OP_glIsVertexArrayOES: {
                GLuint var_array = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glIsVertexArrayOES: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(GLboolean);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glIsVertexArrayOES\n",
                            err);
#endif
                DEBUG("gles1(%p): glIsVertexArrayOES(%u )\n", stream, var_array);
                *(GLboolean *) (&tmpBuf[0]) = this->glIsVertexArrayOES(var_array);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glIsVertexArrayOES");
                break;
            }
            case OP_glDiscardFramebufferEXT: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLsizei var_numAttachments = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                uint32_t size_attachments __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_attachments(ptr + 8 + 4 + 4 + 4, size_attachments);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_attachments,
                                                             ptr + 8 + 4 + 4 + 4 + size_attachments,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDiscardFramebufferEXT: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDiscardFramebufferEXT\n",
                            err);
#endif
                DEBUG("gles1(%p): glDiscardFramebufferEXT(0x%08x %d %p(%u) )\n", stream, var_target,
                      var_numAttachments, (const GLenum *) (inptr_attachments.get()),
                      size_attachments);
                this->glDiscardFramebufferEXT(var_target, var_numAttachments,
                                              (const GLenum *) (inptr_attachments.get()));
                SET_LASTCALL("glDiscardFramebufferEXT");
                break;
            }
            case OP_glMultiDrawArraysEXT: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_first __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_first(ptr + 8 + 4 + 4, size_first);
                uint32_t size_count __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + size_first);
                InputBuffer inptr_count(ptr + 8 + 4 + 4 + size_first + 4, size_count);
                GLsizei var_primcount = Unpack<GLsizei, uint32_t>(
                        ptr + 8 + 4 + 4 + size_first + 4 + size_count);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_first + 4 +
                                                             size_count + 4,
                                                             ptr + 8 + 4 + 4 + size_first + 4 +
                                                             size_count + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMultiDrawArraysEXT: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMultiDrawArraysEXT\n",
                            err);
#endif
                DEBUG("gles1(%p): glMultiDrawArraysEXT(0x%08x %p(%u) %p(%u) %d )\n", stream,
                      var_mode, (const GLint *) (inptr_first.get()), size_first,
                      (const GLsizei *) (inptr_count.get()), size_count, var_primcount);
                this->glMultiDrawArraysEXT(var_mode, (const GLint *) (inptr_first.get()),
                                           (const GLsizei *) (inptr_count.get()), var_primcount);
                SET_LASTCALL("glMultiDrawArraysEXT");
                break;
            }
            case OP_glMultiDrawElementsEXT: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_count __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_count(ptr + 8 + 4 + 4, size_count);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4 + size_count);
                uint32_t size_indices __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + size_count + 4);
                InputBuffer inptr_indices(ptr + 8 + 4 + 4 + size_count + 4 + 4, size_indices);
                GLsizei var_primcount = Unpack<GLsizei, uint32_t>(
                        ptr + 8 + 4 + 4 + size_count + 4 + 4 + size_indices);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_count + 4 + 4 +
                                                             size_indices + 4,
                                                             ptr + 8 + 4 + 4 + size_count + 4 + 4 +
                                                             size_indices + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMultiDrawElementsEXT: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMultiDrawElementsEXT\n",
                            err);
#endif
                DEBUG("gles1(%p): glMultiDrawElementsEXT(0x%08x %p(%u) 0x%08x %p(%u) %d )\n",
                      stream, var_mode, (const GLsizei *) (inptr_count.get()), size_count, var_type,
                      (const GLvoid *const *) (inptr_indices.get()), size_indices, var_primcount);
                this->glMultiDrawElementsEXT(var_mode, (const GLsizei *) (inptr_count.get()),
                                             var_type,
                                             (const GLvoid *const *) (inptr_indices.get()),
                                             var_primcount);
                SET_LASTCALL("glMultiDrawElementsEXT");
                break;
            }
            case OP_glMultiDrawArraysSUN: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_first __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_first(ptr + 8 + 4 + 4, size_first);
                uint32_t size_count __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + size_first);
                InputBuffer inptr_count(ptr + 8 + 4 + 4 + size_first + 4, size_count);
                GLsizei var_primcount = Unpack<GLsizei, uint32_t>(
                        ptr + 8 + 4 + 4 + size_first + 4 + size_count);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_first + 4 +
                                                             size_count + 4,
                                                             ptr + 8 + 4 + 4 + size_first + 4 +
                                                             size_count + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMultiDrawArraysSUN: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMultiDrawArraysSUN\n",
                            err);
#endif
                DEBUG("gles1(%p): glMultiDrawArraysSUN(0x%08x %p(%u) %p(%u) %d )\n", stream,
                      var_mode, (GLint *) (inptr_first.get()), size_first,
                      (GLsizei *) (inptr_count.get()), size_count, var_primcount);
                this->glMultiDrawArraysSUN(var_mode, (GLint *) (inptr_first.get()),
                                           (GLsizei *) (inptr_count.get()), var_primcount);
                SET_LASTCALL("glMultiDrawArraysSUN");
                break;
            }
            case OP_glMultiDrawElementsSUN: {
                GLenum var_mode = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_count __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_count(ptr + 8 + 4 + 4, size_count);
                GLenum var_type = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4 + size_count);
                uint32_t size_indices __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + size_count + 4);
                InputBuffer inptr_indices(ptr + 8 + 4 + 4 + size_count + 4 + 4, size_indices);
                GLsizei var_primcount = Unpack<GLsizei, uint32_t>(
                        ptr + 8 + 4 + 4 + size_count + 4 + 4 + size_indices);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_count + 4 + 4 +
                                                             size_indices + 4,
                                                             ptr + 8 + 4 + 4 + size_count + 4 + 4 +
                                                             size_indices + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glMultiDrawElementsSUN: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glMultiDrawElementsSUN\n",
                            err);
#endif
                DEBUG("gles1(%p): glMultiDrawElementsSUN(0x%08x %p(%u) 0x%08x %p(%u) %d )\n",
                      stream, var_mode, (const GLsizei *) (inptr_count.get()), size_count, var_type,
                      (const GLvoid **) (inptr_indices.get()), size_indices, var_primcount);
                this->glMultiDrawElementsSUN(var_mode, (const GLsizei *) (inptr_count.get()),
                                             var_type, (const GLvoid **) (inptr_indices.get()),
                                             var_primcount);
                SET_LASTCALL("glMultiDrawElementsSUN");
                break;
            }
            case OP_glRenderbufferStorageMultisampleIMG: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLsizei var_samples = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                GLenum var_internalformat = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                GLsizei var_width = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLsizei var_height = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glRenderbufferStorageMultisampleIMG: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glRenderbufferStorageMultisampleIMG\n",
                            err);
#endif
                DEBUG("gles1(%p): glRenderbufferStorageMultisampleIMG(0x%08x %d 0x%08x %d %d )\n",
                      stream, var_target, var_samples, var_internalformat, var_width, var_height);
                this->glRenderbufferStorageMultisampleIMG(var_target, var_samples,
                                                          var_internalformat, var_width,
                                                          var_height);
                SET_LASTCALL("glRenderbufferStorageMultisampleIMG");
                break;
            }
            case OP_glFramebufferTexture2DMultisampleIMG: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_attachment = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLenum var_textarget = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4);
                GLuint var_texture = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLint var_level = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLsizei var_samples = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFramebufferTexture2DMultisampleIMG: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glFramebufferTexture2DMultisampleIMG\n",
                            err);
#endif
                DEBUG("gles1(%p): glFramebufferTexture2DMultisampleIMG(0x%08x 0x%08x 0x%08x %u %d %d )\n",
                      stream, var_target, var_attachment, var_textarget, var_texture, var_level,
                      var_samples);
                this->glFramebufferTexture2DMultisampleIMG(var_target, var_attachment,
                                                           var_textarget, var_texture, var_level,
                                                           var_samples);
                SET_LASTCALL("glFramebufferTexture2DMultisampleIMG");
                break;
            }
            case OP_glDeleteFencesNV: {
                GLsizei var_n = Unpack<GLsizei, uint32_t>(ptr + 8);
                uint32_t size_fences __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_fences(ptr + 8 + 4 + 4, size_fences);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_fences,
                                                             ptr + 8 + 4 + 4 + size_fences,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDeleteFencesNV: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glDeleteFencesNV\n", err);
#endif
                DEBUG("gles1(%p): glDeleteFencesNV(%d %p(%u) )\n", stream, var_n,
                      (const GLuint *) (inptr_fences.get()), size_fences);
                this->glDeleteFencesNV(var_n, (const GLuint *) (inptr_fences.get()));
                SET_LASTCALL("glDeleteFencesNV");
                break;
            }
            case OP_glGenFencesNV: {
                GLsizei var_n = Unpack<GLsizei, uint32_t>(ptr + 8);
                uint32_t size_fences __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_fences(ptr + 8 + 4 + 4, size_fences);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_fences,
                                                             ptr + 8 + 4 + 4 + size_fences,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGenFencesNV: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGenFencesNV\n", err);
#endif
                DEBUG("gles1(%p): glGenFencesNV(%d %p(%u) )\n", stream, var_n,
                      (GLuint *) (inptr_fences.get()), size_fences);
                this->glGenFencesNV(var_n, (GLuint *) (inptr_fences.get()));
                SET_LASTCALL("glGenFencesNV");
                break;
            }
            case OP_glIsFenceNV: {
                GLuint var_fence = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glIsFenceNV: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(GLboolean);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glIsFenceNV\n", err);
#endif
                DEBUG("gles1(%p): glIsFenceNV(%u )\n", stream, var_fence);
                *(GLboolean *) (&tmpBuf[0]) = this->glIsFenceNV(var_fence);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glIsFenceNV");
                break;
            }
            case OP_glTestFenceNV: {
                GLuint var_fence = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glTestFenceNV: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(GLboolean);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glTestFenceNV\n", err);
#endif
                DEBUG("gles1(%p): glTestFenceNV(%u )\n", stream, var_fence);
                *(GLboolean *) (&tmpBuf[0]) = this->glTestFenceNV(var_fence);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glTestFenceNV");
                break;
            }
            case OP_glGetFenceivNV: {
                GLuint var_fence = Unpack<GLuint, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetFenceivNV: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetFenceivNV\n", err);
#endif
                DEBUG("gles1(%p): glGetFenceivNV(%u 0x%08x %p(%u) )\n", stream, var_fence,
                      var_pname, (GLint *) (outptr_params.get()), size_params);
                this->glGetFenceivNV(var_fence, var_pname, (GLint *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetFenceivNV");
                break;
            }
            case OP_glFinishFenceNV: {
                GLuint var_fence = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glFinishFenceNV: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glFinishFenceNV\n", err);
#endif
                DEBUG("gles1(%p): glFinishFenceNV(%u )\n", stream, var_fence);
                this->glFinishFenceNV(var_fence);
                SET_LASTCALL("glFinishFenceNV");
                break;
            }
            case OP_glSetFenceNV: {
                GLuint var_fence = Unpack<GLuint, uint32_t>(ptr + 8);
                GLenum var_condition = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4,
                                                             ptr + 8 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glSetFenceNV: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err) fprintf(stderr, "gles1 Error (pre-call): 0x%X before glSetFenceNV\n", err);
#endif
                DEBUG("gles1(%p): glSetFenceNV(%u 0x%08x )\n", stream, var_fence, var_condition);
                this->glSetFenceNV(var_fence, var_condition);
                SET_LASTCALL("glSetFenceNV");
                break;
            }
            case OP_glGetDriverControlsQCOM: {
                uint32_t size_num __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                GLsizei var_size = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                uint32_t size_driverControls __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetDriverControlsQCOM: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_num;
                totalTmpSize += size_driverControls;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_num(&tmpBuf[0], size_num);
                OutputBuffer outptr_driverControls(&tmpBuf[0 + size_num], size_driverControls);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glGetDriverControlsQCOM\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetDriverControlsQCOM(%p(%u) %d %p(%u) )\n", stream,
                      (GLint *) (outptr_num.get()), size_num, var_size,
                      (GLuint *) (outptr_driverControls.get()), size_driverControls);
                this->glGetDriverControlsQCOM((GLint *) (outptr_num.get()), var_size,
                                              (GLuint *) (outptr_driverControls.get()));
                outptr_num.flush();
                outptr_driverControls.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetDriverControlsQCOM");
                break;
            }
            case OP_glGetDriverControlStringQCOM: {
                GLuint var_driverControl = Unpack<GLuint, uint32_t>(ptr + 8);
                GLsizei var_bufSize = Unpack<GLsizei, uint32_t>(ptr + 8 + 4);
                uint32_t size_length __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                uint32_t size_driverControlString __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glGetDriverControlStringQCOM: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_length;
                totalTmpSize += size_driverControlString;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_length(&tmpBuf[0], size_length);
                OutputBuffer outptr_driverControlString(&tmpBuf[0 + size_length],
                                                        size_driverControlString);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glGetDriverControlStringQCOM\n",
                            err);
#endif
                DEBUG("gles1(%p): glGetDriverControlStringQCOM(%u %d %p(%u) %p(%u) )\n", stream,
                      var_driverControl, var_bufSize, (GLsizei *) (outptr_length.get()),
                      size_length, (GLchar *) (outptr_driverControlString.get()),
                      size_driverControlString);
                this->glGetDriverControlStringQCOM(var_driverControl, var_bufSize,
                                                   (GLsizei *) (outptr_length.get()),
                                                   (GLchar *) (outptr_driverControlString.get()));
                outptr_length.flush();
                outptr_driverControlString.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glGetDriverControlStringQCOM");
                break;
            }
            case OP_glEnableDriverControlQCOM: {
                GLuint var_driverControl = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glEnableDriverControlQCOM: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glEnableDriverControlQCOM\n", err);
#endif
                DEBUG("gles1(%p): glEnableDriverControlQCOM(%u )\n", stream, var_driverControl);
                this->glEnableDriverControlQCOM(var_driverControl);
                SET_LASTCALL("glEnableDriverControlQCOM");
                break;
            }
            case OP_glDisableDriverControlQCOM: {
                GLuint var_driverControl = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glDisableDriverControlQCOM: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glDisableDriverControlQCOM\n",
                            err);
#endif
                DEBUG("gles1(%p): glDisableDriverControlQCOM(%u )\n", stream, var_driverControl);
                this->glDisableDriverControlQCOM(var_driverControl);
                SET_LASTCALL("glDisableDriverControlQCOM");
                break;
            }
            case OP_glExtGetTexturesQCOM: {
                uint32_t size_textures __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8);
                GLint var_maxTextures = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                uint32_t size_numTextures __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glExtGetTexturesQCOM: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_textures;
                totalTmpSize += size_numTextures;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_textures(&tmpBuf[0], size_textures);
                OutputBuffer outptr_numTextures(&tmpBuf[0 + size_textures], size_numTextures);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glExtGetTexturesQCOM\n",
                            err);
#endif
                DEBUG("gles1(%p): glExtGetTexturesQCOM(%p(%u) %d %p(%u) )\n", stream,
                      (GLuint *) (outptr_textures.get()), size_textures, var_maxTextures,
                      (GLint *) (outptr_numTextures.get()), size_numTextures);
                this->glExtGetTexturesQCOM((GLuint *) (outptr_textures.get()), var_maxTextures,
                                           (GLint *) (outptr_numTextures.get()));
                outptr_textures.flush();
                outptr_numTextures.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glExtGetTexturesQCOM");
                break;
            }
            case OP_glExtGetBuffersQCOM: {
                uint32_t size_buffers __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                GLint var_maxBuffers = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                uint32_t size_numBuffers __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glExtGetBuffersQCOM: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_buffers;
                totalTmpSize += size_numBuffers;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_buffers(&tmpBuf[0], size_buffers);
                OutputBuffer outptr_numBuffers(&tmpBuf[0 + size_buffers], size_numBuffers);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glExtGetBuffersQCOM\n",
                            err);
#endif
                DEBUG("gles1(%p): glExtGetBuffersQCOM(%p(%u) %d %p(%u) )\n", stream,
                      (GLuint *) (outptr_buffers.get()), size_buffers, var_maxBuffers,
                      (GLint *) (outptr_numBuffers.get()), size_numBuffers);
                this->glExtGetBuffersQCOM((GLuint *) (outptr_buffers.get()), var_maxBuffers,
                                          (GLint *) (outptr_numBuffers.get()));
                outptr_buffers.flush();
                outptr_numBuffers.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glExtGetBuffersQCOM");
                break;
            }
            case OP_glExtGetRenderbuffersQCOM: {
                uint32_t size_renderbuffers __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8);
                GLint var_maxRenderbuffers = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                uint32_t size_numRenderbuffers __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glExtGetRenderbuffersQCOM: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_renderbuffers;
                totalTmpSize += size_numRenderbuffers;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_renderbuffers(&tmpBuf[0], size_renderbuffers);
                OutputBuffer outptr_numRenderbuffers(&tmpBuf[0 + size_renderbuffers],
                                                     size_numRenderbuffers);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glExtGetRenderbuffersQCOM\n", err);
#endif
                DEBUG("gles1(%p): glExtGetRenderbuffersQCOM(%p(%u) %d %p(%u) )\n", stream,
                      (GLuint *) (outptr_renderbuffers.get()), size_renderbuffers,
                      var_maxRenderbuffers, (GLint *) (outptr_numRenderbuffers.get()),
                      size_numRenderbuffers);
                this->glExtGetRenderbuffersQCOM((GLuint *) (outptr_renderbuffers.get()),
                                                var_maxRenderbuffers,
                                                (GLint *) (outptr_numRenderbuffers.get()));
                outptr_renderbuffers.flush();
                outptr_numRenderbuffers.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glExtGetRenderbuffersQCOM");
                break;
            }
            case OP_glExtGetFramebuffersQCOM: {
                uint32_t size_framebuffers __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8);
                GLint var_maxFramebuffers = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                uint32_t size_numFramebuffers __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glExtGetFramebuffersQCOM: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_framebuffers;
                totalTmpSize += size_numFramebuffers;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_framebuffers(&tmpBuf[0], size_framebuffers);
                OutputBuffer outptr_numFramebuffers(&tmpBuf[0 + size_framebuffers],
                                                    size_numFramebuffers);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glExtGetFramebuffersQCOM\n", err);
#endif
                DEBUG("gles1(%p): glExtGetFramebuffersQCOM(%p(%u) %d %p(%u) )\n", stream,
                      (GLuint *) (outptr_framebuffers.get()), size_framebuffers,
                      var_maxFramebuffers, (GLint *) (outptr_numFramebuffers.get()),
                      size_numFramebuffers);
                this->glExtGetFramebuffersQCOM((GLuint *) (outptr_framebuffers.get()),
                                               var_maxFramebuffers,
                                               (GLint *) (outptr_numFramebuffers.get()));
                outptr_framebuffers.flush();
                outptr_numFramebuffers.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glExtGetFramebuffersQCOM");
                break;
            }
            case OP_glExtGetTexLevelParameterivQCOM: {
                GLuint var_texture = Unpack<GLuint, uint32_t>(ptr + 8);
                GLenum var_face = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLint var_level = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4 + 4 + 4);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glExtGetTexLevelParameterivQCOM: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_params;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_params(&tmpBuf[0], size_params);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glExtGetTexLevelParameterivQCOM\n",
                            err);
#endif
                DEBUG("gles1(%p): glExtGetTexLevelParameterivQCOM(%u 0x%08x %d 0x%08x %p(%u) )\n",
                      stream, var_texture, var_face, var_level, var_pname,
                      (GLint *) (outptr_params.get()), size_params);
                this->glExtGetTexLevelParameterivQCOM(var_texture, var_face, var_level, var_pname,
                                                      (GLint *) (outptr_params.get()));
                outptr_params.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glExtGetTexLevelParameterivQCOM");
                break;
            }
            case OP_glExtTexObjectStateOverrideiQCOM: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLenum var_pname = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                GLint var_param = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glExtTexObjectStateOverrideiQCOM: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glExtTexObjectStateOverrideiQCOM\n",
                            err);
#endif
                DEBUG("gles1(%p): glExtTexObjectStateOverrideiQCOM(0x%08x 0x%08x %d )\n", stream,
                      var_target, var_pname, var_param);
                this->glExtTexObjectStateOverrideiQCOM(var_target, var_pname, var_param);
                SET_LASTCALL("glExtTexObjectStateOverrideiQCOM");
                break;
            }
            case OP_glExtGetTexSubImageQCOM: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                GLint var_level = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                GLint var_xoffset = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4);
                GLint var_yoffset = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLint var_zoffset = Unpack<GLint, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                GLsizei var_width = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4);
                GLsizei var_height = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4);
                GLsizei var_depth = Unpack<GLsizei, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                GLenum var_format = Unpack<GLenum, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                GLenum var_type = Unpack<GLenum, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                uint32_t size_texels __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
                                                             4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4 + 4 + 4 +
                                                             4 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glExtGetTexSubImageQCOM: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_texels;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_texels(&tmpBuf[0], size_texels);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glExtGetTexSubImageQCOM\n",
                            err);
#endif
                DEBUG("gles1(%p): glExtGetTexSubImageQCOM(0x%08x %d %d %d %d %d %d %d 0x%08x 0x%08x %p(%u) )\n",
                      stream, var_target, var_level, var_xoffset, var_yoffset, var_zoffset,
                      var_width, var_height, var_depth, var_format, var_type,
                      (GLvoid *) (outptr_texels.get()), size_texels);
                this->glExtGetTexSubImageQCOM(var_target, var_level, var_xoffset, var_yoffset,
                                              var_zoffset, var_width, var_height, var_depth,
                                              var_format, var_type,
                                              (GLvoid *) (outptr_texels.get()));
                outptr_texels.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glExtGetTexSubImageQCOM");
                break;
            }
            case OP_glExtGetBufferPointervQCOM: {
                GLenum var_target = Unpack<GLenum, uint32_t>(ptr + 8);
                uint32_t size_params __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4);
                InputBuffer inptr_params(ptr + 8 + 4 + 4, size_params);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + size_params,
                                                             ptr + 8 + 4 + 4 + size_params,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glExtGetBufferPointervQCOM: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glExtGetBufferPointervQCOM\n",
                            err);
#endif
                DEBUG("gles1(%p): glExtGetBufferPointervQCOM(0x%08x %p(%u) )\n", stream, var_target,
                      (GLvoid **) (inptr_params.get()), size_params);
                this->glExtGetBufferPointervQCOM(var_target, (GLvoid **) (inptr_params.get()));
                SET_LASTCALL("glExtGetBufferPointervQCOM");
                break;
            }
            case OP_glExtGetShadersQCOM: {
                uint32_t size_shaders __attribute__((unused)) = Unpack<uint32_t, uint32_t>(ptr + 8);
                GLint var_maxShaders = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                uint32_t size_numShaders __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glExtGetShadersQCOM: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_shaders;
                totalTmpSize += size_numShaders;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_shaders(&tmpBuf[0], size_shaders);
                OutputBuffer outptr_numShaders(&tmpBuf[0 + size_shaders], size_numShaders);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glExtGetShadersQCOM\n",
                            err);
#endif
                DEBUG("gles1(%p): glExtGetShadersQCOM(%p(%u) %d %p(%u) )\n", stream,
                      (GLuint *) (outptr_shaders.get()), size_shaders, var_maxShaders,
                      (GLint *) (outptr_numShaders.get()), size_numShaders);
                this->glExtGetShadersQCOM((GLuint *) (outptr_shaders.get()), var_maxShaders,
                                          (GLint *) (outptr_numShaders.get()));
                outptr_shaders.flush();
                outptr_numShaders.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glExtGetShadersQCOM");
                break;
            }
            case OP_glExtGetProgramsQCOM: {
                uint32_t size_programs __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8);
                GLint var_maxPrograms = Unpack<GLint, uint32_t>(ptr + 8 + 4);
                uint32_t size_numPrograms __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glExtGetProgramsQCOM: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = size_programs;
                totalTmpSize += size_numPrograms;
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
                OutputBuffer outptr_programs(&tmpBuf[0], size_programs);
                OutputBuffer outptr_numPrograms(&tmpBuf[0 + size_programs], size_numPrograms);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glExtGetProgramsQCOM\n",
                            err);
#endif
                DEBUG("gles1(%p): glExtGetProgramsQCOM(%p(%u) %d %p(%u) )\n", stream,
                      (GLuint *) (outptr_programs.get()), size_programs, var_maxPrograms,
                      (GLint *) (outptr_numPrograms.get()), size_numPrograms);
                this->glExtGetProgramsQCOM((GLuint *) (outptr_programs.get()), var_maxPrograms,
                                           (GLint *) (outptr_numPrograms.get()));
                outptr_programs.flush();
                outptr_numPrograms.flush();
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glExtGetProgramsQCOM");
                break;
            }
            case OP_glExtIsProgramBinaryQCOM: {
                GLuint var_program = Unpack<GLuint, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glExtIsProgramBinaryQCOM: GL checksumCalculator failure\n");
                }
                size_t totalTmpSize = sizeof(GLboolean);
                totalTmpSize += checksumSize;
                unsigned char *tmpBuf = stream->alloc(totalTmpSize);
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glExtIsProgramBinaryQCOM\n", err);
#endif
                DEBUG("gles1(%p): glExtIsProgramBinaryQCOM(%u )\n", stream, var_program);
                *(GLboolean *) (&tmpBuf[0]) = this->glExtIsProgramBinaryQCOM(var_program);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::writeChecksum(checksumCalc, &tmpBuf[0],
                                                                totalTmpSize - checksumSize,
                                                                &tmpBuf[totalTmpSize -
                                                                        checksumSize],
                                                                checksumSize);
                }
                stream->flush();
                SET_LASTCALL("glExtIsProgramBinaryQCOM");
                break;
            }
            case OP_glExtGetProgramBinarySourceQCOM: {
                GLuint var_program = Unpack<GLuint, uint32_t>(ptr + 8);
                GLenum var_shadertype = Unpack<GLenum, uint32_t>(ptr + 8 + 4);
                uint32_t size_source __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4);
                InputBuffer inptr_source(ptr + 8 + 4 + 4 + 4, size_source);
                uint32_t size_length __attribute__((unused)) = Unpack<uint32_t, uint32_t>(
                        ptr + 8 + 4 + 4 + 4 + size_source);
                InputBuffer inptr_length(ptr + 8 + 4 + 4 + 4 + size_source + 4, size_length);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + size_source + 4 +
                                                             size_length,
                                                             ptr + 8 + 4 + 4 + 4 + size_source + 4 +
                                                             size_length, checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glExtGetProgramBinarySourceQCOM: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr,
                            "gles1 Error (pre-call): 0x%X before glExtGetProgramBinarySourceQCOM\n",
                            err);
#endif
                DEBUG("gles1(%p): glExtGetProgramBinarySourceQCOM(%u 0x%08x %p(%u) %p(%u) )\n",
                      stream, var_program, var_shadertype, (GLchar *) (inptr_source.get()),
                      size_source, (GLint *) (inptr_length.get()), size_length);
                this->glExtGetProgramBinarySourceQCOM(var_program, var_shadertype,
                                                      (GLchar *) (inptr_source.get()),
                                                      (GLint *) (inptr_length.get()));
                SET_LASTCALL("glExtGetProgramBinarySourceQCOM");
                break;
            }
            case OP_glStartTilingQCOM: {
                GLuint var_x = Unpack<GLuint, uint32_t>(ptr + 8);
                GLuint var_y = Unpack<GLuint, uint32_t>(ptr + 8 + 4);
                GLuint var_width = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4);
                GLuint var_height = Unpack<GLuint, uint32_t>(ptr + 8 + 4 + 4 + 4);
                GLbitfield var_preserveMask = Unpack<GLbitfield, uint32_t>(ptr + 8 + 4 + 4 + 4 + 4);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr,
                                                             8 + 4 + 4 + 4 + 4 + 4,
                                                             ptr + 8 + 4 + 4 + 4 + 4 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glStartTilingQCOM: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glStartTilingQCOM\n", err);
#endif
                DEBUG("gles1(%p): glStartTilingQCOM(%u %u %u %u 0x%08x )\n", stream, var_x, var_y,
                      var_width, var_height, var_preserveMask);
                this->glStartTilingQCOM(var_x, var_y, var_width, var_height, var_preserveMask);
                SET_LASTCALL("glStartTilingQCOM");
                break;
            }
            case OP_glEndTilingQCOM: {
                GLbitfield var_preserveMask = Unpack<GLbitfield, uint32_t>(ptr + 8);
                if (useChecksum) {
                    ChecksumCalculatorThreadInfo::validOrDie(checksumCalc, ptr, 8 + 4, ptr + 8 + 4,
                                                             checksumSize,
                                                             "gles1_decoder_context_t::decode, OP_glEndTilingQCOM: GL checksumCalculator failure\n");
                }
#ifdef CHECK_GL_ERRORS
                GLint err = this->glGetError();
                if (err)
                    fprintf(stderr, "gles1 Error (pre-call): 0x%X before glEndTilingQCOM\n", err);
#endif
                DEBUG("gles1(%p): glEndTilingQCOM(0x%08x )\n", stream, var_preserveMask);
                this->glEndTilingQCOM(var_preserveMask);
                SET_LASTCALL("glEndTilingQCOM");
                break;
            }
            default:
                return ptr - (unsigned char *) buf;
        } //switch
#ifdef CHECK_GL_ERRORS
#if LOG_CALL_NAME
        if (strlen(lastCall))
            ALOGE("xxx gles1 call %s", lastCall);
#endif
        GLint err = this->glGetError();
        if (err) ALOGE("gles1 Error (post-call): 0x%X in %s\n", err, lastCall);
#endif
        ptr += packetLen;
    } // while
    return ptr - (unsigned char *) buf;
}

#pragma clang diagnostic pop