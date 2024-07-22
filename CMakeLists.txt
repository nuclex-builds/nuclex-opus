#!/usr/bin/cmake
cmake_minimum_required (VERSION 3.18)

# -------------------------------------------------------------------------------------------------

project(
	Opus
	VERSION 1.4
	DESCRIPTION "Lossy audio codec that beats any other lossy codec there is"
)

# This sets a bunch of compile flags and defined ${NUCLEX_COMPILER_TAG} to
# say something like linux-gcc-13.2-amd64-debug. You should have this directory
# if you do a full clone of a project that is using this third-party library build.
include("../../build-system/cmake/cplusplus.cmake")

# -------------------------------------------------------------------------------------------------

# TODO: Effin' CMake again. Can't this abomination of a useless build system just die?
#  If downloads the file. It complains about the checksum if it's wrong.
# Then it writes a BAD status into DOWNLOAD_STATUS. And leaves ERROR_MESSAGE empty.
# Completely useless. Weird, custom build language with side effects as the primary
# means of returning results and that probably changes between versions. Not fixing this.
if(NOT EXISTS ${PROJECT_SOURCE_DIR}/build)
	message(STATUS "Build directory not present, downloading sources")

	file(
		DOWNLOAD https://github.com/xiph/opus/archive/refs/tags/v1.4.tar.gz
		${PROJECT_SOURCE_DIR}/downloads/v1.4.tar.gz
		SHOW_PROGRESS
		EXPECTED_HASH SHA256=7875d49596286055b52439ed42f044bd8ad426aa4cc5aabd96bfe7abb971d5e8
	)
	list(GET DOWNLOAD_STATUS 0 STATUS_CODE)
	if(NOT ${STATUS_CODE} EQUAL 0)
		list(GET DOWNLOAD_STATUS 1 ERROR_MESSAGE)
		file(REMOVE ${PROJECT_SOURCE_DIR}/downloads/v1.4.tar.gz)
		message(FATAL_ERROR "Could not download Opus: ${ERROR_MESSAGE}")
	endif()

	message(STATUS "Extracting Opus sources")
	file(
		ARCHIVE_EXTRACT
		INPUT ${PROJECT_SOURCE_DIR}/downloads/v1.4.tar.gz
		DESTINATION ${CMAKE_BINARY_DIR}/extract
	)
	file(
		RENAME
		${CMAKE_BINARY_DIR}/extract/v1.4
		${PROJECT_SOURCE_DIR}/build
	)
endif()

# -------------------------------------------------------------------------------------------------

#configure_file(
#	"${PROJECT_SOURCE_DIR}/config.h"
#	"${PROJECT_SOURCE_DIR}/build/config.h"
#	COPYONLY
#)

# -------------------------------------------------------------------------------------------------

set(
  sourceFiles
  "build/src/opus_encoder.c"
  "build/src/opus.c"
  "build/src/opus_multistream.c"
  "build/src/opus_projection_decoder.c"
  "build/src/mapping_matrix.c"
  "build/src/analysis.c"
  "build/src/opus_projection_encoder.c"
  "build/src/opus_decoder.c"
  "build/src/repacketizer.c"
  "build/src/opus_multistream_encoder.c"
  "build/src/mlp.c"
  "build/src/opus_multistream_decoder.c"
  "build/src/mlp_data.c"
  "build/silk/decode_core.c"
  "build/silk/CNG.c"
  "build/silk/init_decoder.c"
  "build/silk/code_signs.c"
  "build/silk/decode_parameters.c"
  "build/silk/decode_frame.c"
  "build/silk/decoder_set_fs.c"
  "build/silk/decode_indices.c"
  "build/silk/dec_API.c"
  "build/silk/gain_quant.c"
  "build/silk/interpolate.c"
  "build/silk/encode_indices.c"
  "build/silk/enc_API.c"
  "build/silk/encode_pulses.c"
  "build/silk/decode_pulses.c"
  "build/silk/LP_variable_cutoff.c"
  "build/silk/NSQ_del_dec.c"
  "build/silk/NLSF_decode.c"
  "build/silk/PLC.c"
  "build/silk/NSQ.c"
  "build/silk/shell_coder.c"
  "build/silk/tables_gain.c"
  "build/silk/tables_LTP.c"
  "build/silk/tables_NLSF_CB_NB_MB.c"
  "build/silk/tables_other.c"
  "build/silk/tables_NLSF_CB_WB.c"
  "build/silk/tables_pitch_lag.c"
  "build/silk/tables_pulses_per_block.c"
  "build/silk/control_audio_bandwidth.c"
  "build/silk/VAD.c"
  "build/silk/quant_LTP_gains.c"
  "build/silk/VQ_WMat_EC.c"
  "build/silk/HP_variable_cutoff.c"
  "build/silk/NLSF_VQ.c"
  "build/silk/NLSF_encode.c"
  "build/silk/NLSF_unpack.c"
  "build/silk/NLSF_del_dec_quant.c"
  "build/silk/process_NLSFs.c"
  "build/silk/stereo_MS_to_LR.c"
  "build/silk/stereo_LR_to_MS.c"
  "build/silk/check_control_input.c"
  "build/silk/control_SNR.c"
  "build/silk/init_encoder.c"
  "build/silk/A2NLSF.c"
  "build/silk/control_codec.c"
  "build/silk/ana_filt_bank_1.c"
  "build/silk/biquad_alt.c"
  "build/silk/bwexpander_32.c"
  "build/silk/bwexpander.c"
  "build/silk/debug.c"
  "build/silk/decode_pitch.c"
  "build/silk/inner_prod_aligned.c"
  "build/silk/lin2log.c"
  "build/silk/log2lin.c"
  "build/silk/LPC_analysis_filter.c"
  "build/silk/table_LSF_cos.c"
  "build/silk/LPC_inv_pred_gain.c"
  "build/silk/NLSF2A.c"
  "build/silk/NLSF_stabilize.c"
  "build/silk/pitch_est_tables.c"
  "build/silk/NLSF_VQ_weights_laroia.c"
  "build/silk/resampler.c"
  "build/silk/resampler_down2_3.c"
  "build/silk/resampler_down2.c"
  "build/silk/resampler_private_down_FIR.c"
  "build/silk/resampler_private_AR2.c"
  "build/silk/resampler_private_IIR_FIR.c"
  "build/silk/sigm_Q15.c"
  "build/silk/resampler_private_up2_HQ.c"
  "build/silk/resampler_rom.c"
  "build/silk/sort.c"
  "build/silk/stereo_decode_pred.c"
  "build/silk/sum_sqr_shift.c"
  "build/silk/stereo_find_predictor.c"
  "build/silk/stereo_encode_pred.c"
  "build/silk/stereo_quant_pred.c"
  "build/silk/LPC_fit.c"
  "build/celt/celt.c"
  "build/celt/bands.c"
  "build/celt/celt_encoder.c"
  "build/celt/celt_decoder.c"
  "build/celt/cwrs.c"
  "build/celt/entcode.c"
  "build/celt/entenc.c"
  "build/celt/entdec.c"
  "build/celt/kiss_fft.c"
  "build/celt/laplace.c"
  "build/celt/mathops.c"
  "build/celt/mdct.c"
  "build/celt/modes.c"
  "build/celt/pitch.c"
  "build/celt/celt_lpc.c"
  "build/celt/quant_bands.c"
  "build/celt/rate.c"
  "build/celt/vq.c"
  "build/silk/float/apply_sine_window_FLP.c"
  "build/silk/float/corrMatrix_FLP.c"
  "build/silk/float/encode_frame_FLP.c"
  "build/silk/float/find_LPC_FLP.c"
  "build/silk/float/find_LTP_FLP.c"
  "build/silk/float/find_pitch_lags_FLP.c"
  "build/silk/float/find_pred_coefs_FLP.c"
  "build/silk/float/LTP_analysis_filter_FLP.c"
  "build/silk/float/LPC_analysis_filter_FLP.c"
  "build/silk/float/LTP_scale_ctrl_FLP.c"
  "build/silk/float/noise_shape_analysis_FLP.c"
  "build/silk/float/process_gains_FLP.c"
  "build/silk/float/regularize_correlations_FLP.c"
  "build/silk/float/residual_energy_FLP.c"
  "build/silk/float/warped_autocorrelation_FLP.c"
  "build/silk/float/wrappers_FLP.c"
  "build/silk/float/autocorrelation_FLP.c"
  "build/silk/float/burg_modified_FLP.c"
  "build/silk/float/bwexpander_FLP.c"
  "build/silk/float/energy_FLP.c"
  "build/silk/float/inner_product_FLP.c"
  "build/silk/float/k2a_FLP.c"
  "build/silk/float/LPC_inv_pred_gain_FLP.c"
  "build/silk/float/pitch_analysis_core_FLP.c"
  "build/silk/float/scale_copy_vector_FLP.c"
  "build/silk/float/schur_FLP.c"
  "build/silk/float/scale_vector_FLP.c"
  "build/celt/x86/x86cpu.c"
  "build/silk/float/sort_FLP.c"
  "build/celt/x86/x86_celt_map.c"
  "build/silk/x86/x86_silk_map.c"
  "build/celt/x86/pitch_sse.c"
  "build/celt/x86/pitch_sse2.c"
  "build/celt/x86/vq_sse2.c"
  "build/celt/x86/celt_lpc_sse4_1.c"
  "build/celt/x86/pitch_sse4_1.c"
  "build/silk/x86/NSQ_sse4_1.c"
  "build/silk/x86/NSQ_del_dec_sse4_1.c"
  "build/silk/x86/VAD_sse4_1.c"
  "build/silk/x86/VQ_WMat_EC_sse4_1.c"
)
set(
  headerFiles
  "build/include/opus_projection.h"
  "build/include/opus_custom.h"
  "build/include/opus_defines.h"
  "build/include/opus_multistream.h"
  "build/include/opus_types.h"
  "build/include/opus.h"
)

# -------------------------------------------------------------------------------------------------

add_library(Opus STATIC)
add_library(Opus::Static ALIAS Opus)

target_compile_definitions(
	Opus
  PRIVATE OPUS_EXPORTS
	PRIVATE OPUS_BUILD
  PRIVATE USE_ALLOCA
)

target_include_directories(
	Opus
	PUBLIC "build/include"
  PRIVATE "build" # Ah yes, four conventions to access one's headers...
  PRIVATE "build/celt"
  PRIVATE "build/silk"
  PRIVATE "build/silk/float"
)

target_sources(
	Opus
	PUBLIC ${headerFiles}
	PRIVATE ${sourceFiles}
)

set_target_properties(Opus PROPERTIES PREFIX "")
set_target_properties(Opus PROPERTIES OUTPUT_NAME "opus")

# -------------------------------------------------------------------------------------------------

install(
	TARGETS Opus
	ARCHIVE	DESTINATION ${PROJECT_SOURCE_DIR}/bin/${NUCLEX_COMPILER_TAG}
)

# -------------------------------------------------------------------------------------------------

file(
	WRITE "${PROJECT_SOURCE_DIR}/OpusConfig.cmake"
	"#!/usr/bin/cmake

# Configuration to include Opus in a CMake-based project. If you want to
# reference Opus as an externally compiled static library, do this:
#
#    set(Opus_DIR \"../ThirdParty/opus\")
#    find_package(Opus REQUIRED CONFIG)
#
#    target_link_libraries(
#        MyAwesomeProject
#        PRIVATE Opus::Static
#    )
#
# Alternatively, if you want to build Opus together with your project,
# use the normal CMakeLists.txt with CMake's add_subdirectory() command:
#
#    add_subdirectory(
#        \"\${PROJECT_SOURCE_DIR}/../ThirdParty/opus\"
#        \"\${CMAKE_BINARY_DIR}/opus\"
#    )
#
#    target_link_libraries(
#        MyAwesomeProject
#        PRIVATE Opus
#    )
#

# -------------------------------------------------------------------------------------------------

if(NOT DEFINED NUCLEX_COMPILER_TAG)
	message(
		FATAL_ERROR
		\"NUCLEX_COMPILER_TAG not defined! Include cplusplus.cmake before importing this package \\
		in order to generate a tag identifying the platform/compiler/architecture/variant!\"
	)
endif()

# -------------------------------------------------------------------------------------------------

if(NOT EXISTS \"\${CMAKE_CURRENT_LIST_DIR}/bin/\${NUCLEX_COMPILER_TAG}\")
	# TODO: Warn and link release build when compiling in debug mode
	# TODO: Warn and link build for older compiler version if found
	message(
		FATAL_ERROR
		\"Directory '\${CMAKE_CURRENT_LIST_DIR}/bin/\${NUCLEX_COMPILER_TAG}' not found. \\
		Please either build and install this project before importing it via \\
		find_package() or use this project's main CMakeFiles.txt via add_subdirectory()!\"
	)
endif()

# -------------------------------------------------------------------------------------------------

add_library(Opus::Static STATIC IMPORTED)

# This may cause warnings on recent GCC versions (10.0.0+?) with LTO because GCC detects
# that the headers used during build (residing in build/) are not the same used when
# linking the library (copies resising in Include/).
#
# CMake doesn't run the install step during build, so the only way to get the headers
# in place before building would be by copying them rather than installing them.
set_target_properties(
	Opus::Static PROPERTIES
	INTERFACE_INCLUDE_DIRECTORIES \"\${CMAKE_CURRENT_LIST_DIR}/Include\"
	IMPORTED_LINK_INTERFACE_LANGUAGES \"C\"
)

if(WIN32)
	set_target_properties(
		Opus::Static PROPERTIES
		IMPORTED_LOCATION \"\${CMAKE_CURRENT_LIST_DIR}/bin/\${NUCLEX_COMPILER_TAG}/opus.lib\"
	)
else()
	set_target_properties(
		Opus::Static PROPERTIES
		IMPORTED_LOCATION \"\${CMAKE_CURRENT_LIST_DIR}/bin/\${NUCLEX_COMPILER_TAG}/opus.a\"
	)
endif()

message(STATUS \"Imported Opus targets with binaries in '\${CMAKE_CURRENT_LIST_DIR}'\")"
)

# -------------------------------------------------------------------------------------------------
