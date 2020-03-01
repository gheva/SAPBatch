# Targets for specific build artefacts 

# Build C++ into an object, store it's dependencies
${OUTDIR}/%.$(strip ${OBJ_SFX}):${SRCDIR}%.cpp
	$(call MESSAGE, Building $@)
	${QQ}${CXX} ${CXXFLAGS} ${CFLAGS} ${$(strip $@)_FLAGS} -fPIC -c -o $@ $<
	@${CXX} ${CXXFLAGS} ${CFLAGS} ${$(strip $@)_FLAGS} -MM $< | sed 's;^[^:]*:;$@:;' > ${OUTDIR}/$*.d

${OUTDIR}/%.$(strip ${OBJ_SFX}):${SRCDIR}%.c
	$(call MESSAGE, Building $@)
	${QQ}${CC} ${CFLAGS} ${$(strip $@)_FLAGS} -fPIC -c -o $@ $<
	@${CC} ${CFLAGS} ${$(strip $@)_FLAGS} -MM $< | sed 's;^[^:]*:;$@:;' > ${OUTDIR}/$*.d


# Link shared libraries
%.$(strip ${SHARED_SFX}):
	$(call MESSAGE, Linking shared $@)
	${QQ}$(CXX) $(CXXFLAGS) $(CFLAGS) $(LDFLAGS) -shared -o $@ ${ADD_ARCHIVE_FLAG} $^ ${ARCHIVES_ADDED_FLAG} $($(strip $(basename $(notdir $@)))_FLAGS)

# Link static libraries
%.$(strip ${STATIC_SFX}):
	$(call MESSAGE, Linking static $@)
	${QQ}ar cr $@ $^

# Copy exported headers
$(strip ${INCLUDIR})/%.h:${SRCDIR}%.h
	$(call MESSAGE, Copying header $@)
	${QQ}cp $< $@

# vim: set noexpandtab :

